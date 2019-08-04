// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCArea.h"

#include "WFCUtil.h"
#include <DrawDebugHelpers.h>
#include <string>

// Sets default values
AWFCArea::AWFCArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridSize = 160;

	PlayerPos.x = 0;
	PlayerPos.y = 0;
	PlayerPos.z = 0;

	LightSize = 1;
}

bool AWFCArea::CanOverlap(int x, int y, int z)
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

FWFCIntVector AWFCArea::GetMovealbePos(const FWFCIntVector& TargetPos)
{
	std::string HashCode = TargetPos.GetHash();
	if (AllBlockGrid.count(HashCode) > 0 && !AllBlockGrid[HashCode]->CanOverlap())
	{
		//被阻挡
		return FWFCIntVector(-1, -1, -1);
	}
	else
	{
		//向下查找
		for (int i = -1; i > -LightSize; i--)
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

// Called when the game starts or when spawned
void AWFCArea::BeginPlay()
{
	Super::BeginPlay();

	initArea();
}

void AWFCArea::initArea()
{
	UpdateLightedBlocksGridPos();
	AddNewGridPos();
}

void AWFCArea::MoveToDirection(FWFCIntVector Direction)
{
	PlayerPos = PlayerPos + Direction;

	//	DrawDebugSphere(GetWorld(), FVector(PlayerPos.x*GridSize,PlayerPos.y*GridSize,PlayerPos.z*GridSize), LightSize*GridSize, 12, FColor::Red, false, 2.0f, 0, 1.0f);

	UpdateLightedBlocksGridPos();
	RemoveOutBlocks();
	AddNewGridPos();

	//
	//
	//	for (int i=0;i<AllBlock.Num();i++)
	//	{
	//		WFCIntVec BlockIndex = GetBlockIndex(i);
	//		WFCIntVec Pos = GetReletivePos(BlockIndex.x, BlockIndex.y, BlockIndex.z);
	//		Pos = Pos + Direction;
	//		TWeakObjectPtr<AWFCBlock> Block = AllBlock[i];
	//		Block->SetActorLocation(FVector(Pos.x*GridSize, Pos.y*GridSize, Pos.z*GridSize));
	//	}
}

void AWFCArea::MoveForward()
{
	MoveToDirection(FWFCIntVector(-1, 0, 0));
}

void AWFCArea::MoveBack()
{
	MoveToDirection(FWFCIntVector(1, 0, 0));
}

void AWFCArea::MoveRight()
{
	MoveToDirection(FWFCIntVector(0, -1, 0));
}

void AWFCArea::MoveLeft()
{
	MoveToDirection(FWFCIntVector(0, 1, 0));
}

void AWFCArea::MoveUp()
{
	MoveToDirection(FWFCIntVector(0, 0, 1));
}

void AWFCArea::MoveDown()
{
	MoveToDirection(FWFCIntVector(0, 0, -1));
}

//TWeakObjectPtr<AWFCBlock> AWFCArea::GetBlock(int x, int y, int z)
//{
//	return AllBlock[z + y * AreaSize.z + x * AreaSize.y*AreaSize.z];
//}
//
//WFCIntVector AWFCArea::GetBlockIndex(int Index)
//{
//	WFCIntVector ivec;
//	int temp = AreaSize.z*AreaSize.y;
//	ivec.x = Index / temp;
//	ivec.y = (Index%temp) / AreaSize.z;
//	ivec.z = (Index%temp) % AreaSize.z;
//	return ivec;
//}
//
//void AWFCArea::UpdateBlockPosition()
//{
//	for (int i = 0; i < AreaSize.GetVolume(); i++)
//	{
//		WFCIntVector BlockIndex = GetBlockIndex(i);
//		WFCIntVector Pos = GetReletivePos(BlockIndex.x, BlockIndex.y, BlockIndex.z);
//		TWeakObjectPtr<AWFCBlock> Block = AllBlock[i];
//		Block->SetActorLocation(FVector(Pos.x*GridSize, Pos.y*GridSize, Pos.z*GridSize));
//	}
//}

//WFCIntVector AWFCArea::GetReletivePos(int x, int y, int z)
//{
//	WFCIntVector ivec;
//	ivec.x = GetReletiveIndex(x, PlayerPos.x, AreaSize.x);
//	ivec.y = GetReletiveIndex(y, PlayerPos.y, AreaSize.y);
//	ivec.z = GetReletiveIndex(z, PlayerPos.z, AreaSize.z);
//	return ivec;
//}

int AWFCArea::GetReletiveIndex(int Index, int PlayerIndex, int Length)
{
	int delta = Index - PlayerIndex;
	int sign = delta < 0 ? -1 : 1;

	if (abs(delta) <= 5)
	{
		return delta;
	}
	else
	{
		return (sign)*(Length - abs(delta));
	}
}

// Called every frame
void AWFCArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < 5; i++)
	{
		AddBlockToScene();
	}
}

void AWFCArea::UpdateLightedBlocksGridPos()
{
	LightedBlocksGridPos.Empty();

	for (int i = -LightSize; i <= LightSize; i++)
	{
		for (int j = -LightSize; j <= LightSize; j++)
		{
			for (int k = -LightSize; k <= LightSize; k++)
			{
				FWFCIntVector Pos(PlayerPos.x + i, PlayerPos.y + j, PlayerPos.z + k);

				if (PlayerPos.GetDistance(Pos) <= LightSize
					&& k < 0)
				{
					LightedBlocksGridPos.Emplace(Pos);
				}
			}
		}
	}
}

void AWFCArea::RemoveOutBlocks()
{
	//移除场景中的block
	TArray<TWeakObjectPtr<AWFCBlock> > TempArray;
	//NeedToBlockGridPos.Empty();

	auto BlocksIter = AllBlockGrid.begin();
	while (BlocksIter != AllBlockGrid.end())
	{
		bool SearchFlag = false;
		for (int j = 0; j < LightedBlocksGridPos.Num(); j++)
		{
			FWFCIntVector BlockGridPos = BlocksIter->second->GridPos;
			FWFCIntVector LightGridPos = LightedBlocksGridPos[j];

			if (BlockGridPos == LightGridPos)
			{
				SearchFlag = true;
				break;
			}
		}

		if (!SearchFlag)
		{
			//			AllBlock[i]->SetActorHiddenInGame(true);
//			AllBlock[i]->Destroy();
			BlocksIter->second->ClearBlocks();
			BlocksIter = AllBlockGrid.erase(BlocksIter);
		}
		else
		{
			BlocksIter++;
		}
	}

	//移除待生成block，防止生成无效的block
	auto iter = NeedToBlockGridPos.begin();
	while (iter != NeedToBlockGridPos.end())
	{
		bool SearchFlag = false;
		for (int j = 0; j < LightedBlocksGridPos.Num(); j++)
		{
			FWFCIntVector BlockGridPos = *iter;
			FWFCIntVector LightGridPos = LightedBlocksGridPos[j];

			if (BlockGridPos == LightGridPos)
			{
				SearchFlag = true;
				break;
			}
		}

		if (!SearchFlag)

		{
			
			iter = NeedToBlockGridPos.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void AWFCArea::AddNewGridPos()
{
	for (int i = 0; i < LightedBlocksGridPos.Num(); i++)
	{
		bool SearchFlag = false;

		//搜索场景中是否存在
		if (AllBlockGrid.count(LightedBlocksGridPos[i].GetHash()) > 0)
		{
			SearchFlag = true;
		}

		//搜索是否已经加入了待生成block队列
		auto iter = NeedToBlockGridPos.begin();
		while (iter != NeedToBlockGridPos.end())
		{
			FWFCIntVector LightGridPos = LightedBlocksGridPos[i];
			FWFCIntVector BlockGridPos = *iter;

			if (BlockGridPos == LightGridPos)
			{
				SearchFlag = true;
				break;
			}
			iter++;
		}

		if (!SearchFlag)
		{
			NeedToBlockGridPos.push_back(LightedBlocksGridPos[i]);
		}
	}
}

void AWFCArea::AddBlockToScene()
{
	if (NeedToBlockGridPos.empty())
	{
		return;
	}

	//	if (GEngine) {
	//		GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, std::to_string(NeedToBlockGridPos.size()).c_str());
	//	}

	auto minIter = NeedToBlockGridPos.begin();
	auto iter = NeedToBlockGridPos.begin();
	while (iter != NeedToBlockGridPos.end())
	{
		if (iter != minIter && iter->GetDistanceD(PlayerPos) < minIter->GetDistanceD(PlayerPos))
		{
			minIter = iter;
		}
		iter++;
	}
	GenerateBlockAtPos(*minIter);
	NeedToBlockGridPos.erase(minIter);
}

void AWFCArea::GenerateBlockAtPos(const FWFCIntVector& Pos)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		AWFCBlock* block;
		int rand = WFCUtil::getRandom(0, 1);

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (rand == 0)
		{
			block = World->SpawnActor<AWFCBlock>(BlockClass1, FVector(0, 0, 0), FRotator(0, 0, 0),ActorSpawnParams);
		}
		else
		{
			block = World->SpawnActor<AWFCBlock>(BlockClass2, FVector(0, 0, 0), FRotator(0, 0, 0),ActorSpawnParams);
		}

		if (block)
		{
			AddBlockAtPos(block, Pos);
		}
	}
}

void AWFCArea::AddBlockAtPos(AWFCBlock* Block, const FWFCIntVector& Pos)
{
	Block->SetActorLocation(FVector(Pos.x*GridSize, Pos.y*GridSize, Pos.z*GridSize));
	Block->GridPos = Pos;

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