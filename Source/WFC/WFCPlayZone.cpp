// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCPlayZone.h"

#include "WFCUtil.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AWFCPlayZone::AWFCPlayZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridSize = 160;
}

bool AWFCPlayZone::CanOverlap(int x, int y, int z)
{
	FWFCIntVector ivec(x, y, z);
	if (AllBlockGrid.count(ivec.GetHash()) > 0)
	{
		return AllBlockGrid[ivec.GetHash()]->CanOverlap();
	}
	else
	{
		return true;
	}
}

FWFCIntVector AWFCPlayZone::GetMovealbePos(const FWFCIntVector& TargetPos)
{
	//	std::string temp = std::to_string(PlayerPos.x) + std::to_string(PlayerPos.y) + std::to_string(PlayerPos.z);
	//	if (GEngine) {
	//		GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red,
	//			(temp.c_str())
	//			);
	//	}

	std::string HashCode = TargetPos.GetHash();
	if (AllBlockGrid.count(HashCode) > 0 && !AllBlockGrid[HashCode]->CanOverlap())
	{
		//被阻挡
		return FWFCIntVector(-1, -1, -1);
	}
	else
	{
		int SearchLength = 10;
		//向下查找
		for (int i = -1; i >= -SearchLength; i--)
		{
			FWFCIntVector TempVec = TargetPos;
			TempVec.z = TempVec.z + i;
			std::string TempHashCode = TempVec.GetHash();

			if (AllBlockGrid.count(TempHashCode) > 0)
			{
				if (AllBlockGrid[TempHashCode]->CanStep())
				{
					//下方的block可以立足
					return FWFCIntVector(TempVec.x, TempVec.y, TempVec.z + 1);
				}
				else if (!AllBlockGrid[TempHashCode]->CanStep())
				{
					//下方的block不能立足
					return FWFCIntVector(-1, -1, -1);
				}
			}
		}
	}
	return FWFCIntVector(-1, -1, -1);
}

//FWFCIntVector AWFCPlayZone::GetMovealbePosByPlayerOrientation(EWFCOrientation Ori)
//{
//	FWFCIntVector TargetPos = GirdPos;
//
//	switch (Ori)
//	{
//	case EWFCOrientation::E_UP:
//		TargetPos.x = TargetPos.x + 1;
//		break;
//	case EWFCOrientation::E_DOWN:
//		TargetPos.x = TargetPos.x - 1;
//		break;
//	case EWFCOrientation::E_LEFT:
//		TargetPos.y = TargetPos.y + 1;
//		break;
//	case EWFCOrientation::E_RIGHT:
//		TargetPos.y = TargetPos.y - 1;
//		break;
//	default:
//		break;
//	}
//
//	return GetMovealbePos(TargetPos);
//}

void AWFCPlayZone::AddLightAtGridPos(AWFCLightBlock* Light, const FWFCIntVector& Pos)
{
	AllLight.push_back(Light);
}

void AWFCPlayZone::NextTurn()
{
	Player->EndTurn();
	auto iter = PlayPawns.begin();
	while (iter != PlayPawns.end())
	{
		(*iter)->StartTurn();
		(*iter)->ActionTurn();
		(*iter)->EndTurn();
		iter++;
	}
	Player->StartTurn();
}

void AWFCPlayZone::StartTurn()
{
	auto iter = AllBlockGrid.begin();
	while (iter != AllBlockGrid.end())
	{
		iter->second->StartTurn();
		iter++;
	}
}

void AWFCPlayZone::ActionTurn()
{
	auto iter = AllBlockGrid.begin();
	while (iter != AllBlockGrid.end())
	{
		iter->second->ActionTurn();
		iter++;
	}
}

void AWFCPlayZone::EndTurn()
{
	auto iter = AllBlockGrid.begin();
	while (iter != AllBlockGrid.end())
	{
		iter->second->EndTurn();
		iter++;
	}
}

std::unordered_map<std::string, GridPtr>& AWFCPlayZone::GetAllBlockGridRef()
{
	return AllBlockGrid;
}

// Called when the game starts or when spawned
void AWFCPlayZone::BeginPlay()
{
	Super::BeginPlay();

	initGame();

	Player->ActionTurn();

	NextTurn();
}

void AWFCPlayZone::initGame()
{
	Player=GenerateBlockAtPos(PlayerClass, FWFCIntVector(0, 0, 0));

	APlayerController* PlayerController= UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->Possess(Player);
}

//void AWFCPlayZone::UpdateAllLight()
//{
////	//更新所有光源的照亮区域
////	auto iter = AllLight.begin();
////	while (iter != AllLight.end())
////	{
////		iter->UpdateLightedBlocksGridPos();
////		iter++;
////	}
//}

// Called every frame
void AWFCPlayZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	//移除点亮区域外的grid
	//	RemoveOutBlocks();
	//	//新增点亮但是未生成的grid
	//	AddNewGridPos();

	for (int i = 0; i < 5; i++)
	{
		GenerateBlockToScene();
	}
}

AWFCBlock* AWFCPlayZone::GenerateBlockAtPos(UClass* Class, const FWFCIntVector& Pos)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AWFCBlock* block = World->SpawnActor<AWFCBlock>(Class, FVector(0, 0, 0), FRotator(0, 0, 0), ActorSpawnParams);

		if (block)
		{
			AddBlockAtPos(block, Pos);

			return block;
		}
	}
	return nullptr;
}

bool AWFCPlayZone::IsInLight(const FWFCIntVector& Pos)
{
	//	return AllBlockGrid[Pos.GetHash()];
	return true;
}

void AWFCPlayZone::AddBlockAtPos(AWFCBlock* Block, const FWFCIntVector& Pos)
{
	Block->SetActorLocation(FVector(Pos.x*GridSize, Pos.y*GridSize, Pos.z*GridSize));
	Block->GridPos = Pos;
	Block->PlayZone = this;

	std::string PosHash = Pos.GetHash();
	if (AllBlockGrid.count(PosHash) > 0)
	{
		AllBlockGrid[PosHash]->AddBlock(Block);
	}
	else
	{
		AllBlockGrid[PosHash] = GridPtr(new WFCGrid(Pos));
		AllBlockGrid[PosHash]->AddBlock(Block);
	}
}

void AWFCPlayZone::RemoveOutBlocks()
{
	//移除场景中的Block
	auto BlocksIter = AllBlockGrid.begin();
	while (BlocksIter != AllBlockGrid.end())
	{
		//对于每个grid，检测是否点亮
		FWFCIntVector BlockGridPos = BlocksIter->second->GridPos;
		if (!IsInLight(BlockGridPos))
		{
			BlocksIter->second->ClearBlocks();
			BlocksIter = AllBlockGrid.erase(BlocksIter);
		}
		else
		{
			BlocksIter++;
		}
	}

	//移除待生成队列中block，防止生成无效的block
	auto iter = NeedToBlockGridPosQueue.begin();
	while (iter != NeedToBlockGridPosQueue.end())
	{
		FWFCIntVector BlockGridPos = iter->Pos;

		if (!IsInLight(BlockGridPos))

		{
			iter = NeedToBlockGridPosQueue.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void AWFCPlayZone::LightGridPos(int LightValue, const FWFCIntVector& LightPos, const FWFCIntVector& Pos)
{
	//目标亮度值=光源亮度值-距离衰减
	int TargetLightValue = LightValue - Pos.GetDistance(LightPos);

	if (TargetLightValue <= 0)
	{
		return;
	}

	//搜索场景中是否存在，如果已经存在则修改亮度值
	if (AllBlockGrid.count(Pos.GetHash()) > 0)
	{
		if (AllBlockGrid[Pos.GetHash()]->LightValue < TargetLightValue)
		{
			AllBlockGrid[Pos.GetHash()]->LightValue = TargetLightValue;
		}
		return;
	}

	//搜索是否已经加入了待生成block队列
	bool SearchFlag = false;
	auto iter = NeedToBlockGridPosQueue.begin();
	while (iter != NeedToBlockGridPosQueue.end())
	{
		FWFCIntVector BlockGridPos = iter->Pos;

		if (BlockGridPos == Pos)
		{
			SearchFlag = true;
			break;
		}
		iter++;
	}

	//如果不存在则新增与待生成grid队列中
	if (!SearchFlag)
	{
		NeedToBlockPos NewPos;
		NewPos.Pos = Pos;
		NewPos.LightValueD = double(LightValue) - Pos.GetDistanceD(LightPos);
		NewPos.LightValue = TargetLightValue;
		NeedToBlockGridPosQueue.push_back(NewPos);
	}
}

void AWFCPlayZone::GenerateBlockToScene()
{
	if (NeedToBlockGridPosQueue.empty())
	{
		return;
	}

	//	if (GEngine) {
	//		GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, std::to_string(NeedToBlockGridPos.size()).c_str());
	//	}

	auto minIter = NeedToBlockGridPosQueue.begin();
	auto iter = NeedToBlockGridPosQueue.begin();
	while (iter != NeedToBlockGridPosQueue.end())
	{
		if (iter != minIter && iter->LightValueD < minIter->LightValueD)
		{
			minIter = iter;
		}
		iter++;
	}

	int rand = WFCUtil::getRandom(0, 1);
	if (rand == 0)
	{
		GenerateBlockAtPos(BlockClass1, minIter->Pos);
	}
	else
	{
		GenerateBlockAtPos(BlockClass2, minIter->Pos);
	}

	NeedToBlockGridPosQueue.erase(minIter);
}
