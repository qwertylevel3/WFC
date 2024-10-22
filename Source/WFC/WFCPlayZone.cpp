// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCPlayZone.h"

#include "WFCUtil.h"
#include <Kismet/GameplayStatics.h>
#include "Macro.h"

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

FWFCIntVector AWFCPlayZone::GetMovealbePos(const FWFCIntVector& OriPos, const FWFCIntVector& TargetPos)
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
		return OriPos;
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
					return OriPos;
				}
			}
		}
	}
	return OriPos;
}

FWFCIntVector AWFCPlayZone::GetMovealbePosByDirection(const FWFCIntVector& OriPos, EWFCOrientation Direction)
{
	FWFCIntVector TargetPos = OriPos;
	switch (Direction)
	{
	case EWFCOrientation::E_UP:
		TargetPos.x = TargetPos.x - 1;
		break;
	case EWFCOrientation::E_DOWN:
		TargetPos.x = TargetPos.x + 1;
		break;
	case EWFCOrientation::E_LEFT:
		TargetPos.y = TargetPos.y + 1;
		break;
	case EWFCOrientation::E_RIGHT:
		TargetPos.y = TargetPos.y - 1;
		break;
	default:
		break;
	}
	return GetMovealbePos(OriPos, TargetPos);
}

void AWFCPlayZone::NextTurn()
{
	if (Player)
	{
		Player->ActionTurn();
		Player->EndTurn();
		auto iter = PlayPawns.begin();
		while (iter != PlayPawns.end())
		{
			(*iter)->StartTurn();
			(*iter)->ActionTurn();
			(*iter)->EndTurn();
			iter++;
		}

		CheckLight();

		Player->StartTurn();
	}
}

void AWFCPlayZone::MoveBlock(AWFCBlock* Block, const FWFCIntVector& OriPos, const FWFCIntVector& TargetPos)
{
	std::string OriHashCode = OriPos.GetHash();
	std::string TargetHashCode = TargetPos.GetHash();
	if (AllBlockGrid.count(OriHashCode) > 0)
	{
		AllBlockGrid[OriHashCode]->RemoveBlock(Block);
		//		AddBlockAtPos(Block, TargetPos);
		AddBlockAtPosWithoutWorldLocation(Block, TargetPos);
		Block->MoveAnimation(OriPos, TargetPos);
	}
}

void AWFCPlayZone::TryMoveBlockByDirection(AWFCBlock* Block, const FWFCIntVector& OriPos, EWFCOrientation Direction)
{
	FWFCIntVector TargetPos = GetMovealbePosByDirection(OriPos, Direction);
	if (TargetPos != OriPos)
	{
		MoveBlock(Block, OriPos, TargetPos);
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

	NextTurn();
}

void AWFCPlayZone::initGame()
{
	NeedToGridPos PosInfo;
	PosInfo.LightValue = 2;
	PosInfo.Pos.x = 0;
	PosInfo.Pos.y = 0;
	PosInfo.Pos.z = 0;
	GenerateGridAtPos(PosInfo);
	Player = GenerateBlockAtPos(PlayerClass, PosInfo.Pos);

	if (Player)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->Possess(Player);
	}
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
		GenerateGridToScene();
	}
}

AWFCBlock* AWFCPlayZone::GenerateBlockAtPos(UClass* Class, const FWFCIntVector& Pos)
{
	if (AllBlockGrid.count(Pos.GetHash()) <= 0)
	{
		return nullptr;
	}

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AWFCBlock* Block = World->SpawnActor<AWFCBlock>(Class, FVector(0, 0, 0), FRotator(0, 0, 0), ActorSpawnParams);

		if (Block)
		{
			Block->SetActorLocation(FVector(Pos.x*GridSize, Pos.y*GridSize, Pos.z*GridSize));
			Block->GridPos = Pos;
			Block->PlayZone = this;

			std::string PosHash = Pos.GetHash();
			AllBlockGrid[PosHash]->AddBlock(Block);

			return Block;
		}
	}
	return nullptr;
}

void AWFCPlayZone::GenerateGridAtPos(NeedToGridPos PosInfo)
{
	std::string PosHash = PosInfo.Pos.GetHash();
	if (AllBlockGrid.count(PosHash) <= 0)
	{
		AllBlockGrid[PosHash] = GridPtr(new WFCGrid(PosInfo.Pos, PosInfo.LightValue));
	}
}

void AWFCPlayZone::CheckLight()
{
	auto iter = AllBlockGrid.begin();
	while (iter != AllBlockGrid.end())
	{
		iter->second->CheckLight();
		iter++;
	}
	RemoveOutBlocks();
}

void AWFCPlayZone::AddBlockAtPos(AWFCBlock* Block, const FWFCIntVector& Pos)
{
	Block->SetActorLocation(FVector(Pos.x*GridSize, Pos.y*GridSize, Pos.z*GridSize));
	AddBlockAtPosWithoutWorldLocation(Block, Pos);
}

void AWFCPlayZone::AddBlockAtPosWithoutWorldLocation(AWFCBlock* Block, const FWFCIntVector& Pos)
{
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
		if (BlocksIter->second->LightValue < 0)
		{
			BlocksIter->second->ClearBlocks();
			BlocksIter = AllBlockGrid.erase(BlocksIter);
		}
		else
		{
			BlocksIter++;
		}
	}

	//	//移除待生成队列中block，防止生成无效的block
	//	auto iter = NeedToBlockGridPosQueue.begin();
	//	while (iter != NeedToBlockGridPosQueue.end())
	//	{
	//		FWFCIntVector BlockGridPos = iter->Pos;
	//
	//		if (!IsInLight(BlockGridPos))
	//
	//		{
	//			iter = NeedToBlockGridPosQueue.erase(iter);
	//		}
	//		else
	//		{
	//			iter++;
	//		}
	//	}
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

	//搜索是否已经加入了待生成Grid队列
	bool SearchFlag = false;
	auto iter = NeedToGridPosQueue.begin();
	while (iter != NeedToGridPosQueue.end())
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
		NeedToGridPos NewPos;
		NewPos.Pos = Pos;
		NewPos.LightValueD = double(LightValue) - Pos.GetDistanceD(LightPos);
		NewPos.LightValue = TargetLightValue;
		NeedToGridPosQueue.push_back(NewPos);
	}
}

void AWFCPlayZone::GenerateGridToScene()
{
	if (NeedToGridPosQueue.empty())
	{
		return;
	}

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, std::to_string(NeedToGridPosQueue.size()).c_str());
	}

	auto maxIter = NeedToGridPosQueue.begin();
	auto iter = NeedToGridPosQueue.begin();
	while (iter != NeedToGridPosQueue.end())
	{
		if (iter != maxIter && iter->LightValueD > maxIter->LightValueD)
		{
			maxIter = iter;
		}
		iter++;
	}

	std::string PosHash = maxIter->Pos.GetHash();
	if (AllBlockGrid.count(PosHash) <= 0)
	{
		AllBlockGrid[PosHash] = GridPtr(new WFCGrid(maxIter->Pos, maxIter->LightValue));
	}
	if (maxIter->Pos.z < 0)
	{
		int rand = WFCUtil::getRandom(0, 1);
		if (rand == 0)
		{
			GenerateBlockAtPos(BlockClass1, maxIter->Pos);
		}
		else
		{
			GenerateBlockAtPos(BlockClass2, maxIter->Pos);
		}
	}

	NeedToGridPosQueue.erase(maxIter);
}