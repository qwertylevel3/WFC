// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCArea.h"

#include "WFCUtil.h"
#include <DrawDebugHelpers.h>

// Sets default values
AWFCArea::AWFCArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridSize = 160;

	PlayerPos.x = 0;
	PlayerPos.y = 0;
	PlayerPos.z = 0;

	LightSize = 5;
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
	for (int i = 0; i < LightedBlocksGridPos.Num(); i++)
	{
		if (BlockClass1 != NULL && BlockClass2 != NULL)
		{
			GenerateBlockAtPos(LightedBlocksGridPos[i]);
		}
	}
}

void AWFCArea::MoveToDirection(WFCIntVector Direction)
{
	PlayerPos = PlayerPos + Direction;

	//	DrawDebugSphere(GetWorld(), FVector(PlayerPos.x*GridSize,PlayerPos.y*GridSize,PlayerPos.z*GridSize), LightSize*GridSize, 12, FColor::Red, false, 2.0f, 0, 1.0f);

	UpdateLightedBlocksGridPos();
	RemoveOutBlocks();
	AddNewBlocks();

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
	MoveToDirection(WFCIntVector(-1, 0, 0));
}

void AWFCArea::MoveBack()
{
	MoveToDirection(WFCIntVector(1, 0, 0));
}

void AWFCArea::MoveRight()
{
	MoveToDirection(WFCIntVector(0, -1, 0));
}

void AWFCArea::MoveLeft()
{
	MoveToDirection(WFCIntVector(0, 1, 0));
}

void AWFCArea::MoveUp()
{
	MoveToDirection(WFCIntVector(0, 0, 1));
}

void AWFCArea::MoveDown()
{
	MoveToDirection(WFCIntVector(0, 0, -1));
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
				WFCIntVector Pos(PlayerPos.x + i, PlayerPos.y + j, PlayerPos.z + k);

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
	TArray<TWeakObjectPtr<AWFCBlock> > TempArray;
	NeedToBlockGridPos.Empty();
	for (int i = 0; i < AllBlock.Num(); i++)
	{
		bool SearchFlag = false;
		for (int j = 0; j < LightedBlocksGridPos.Num(); j++)
		{
			WFCIntVector BlockGridPos = AllBlock[i]->GridPos;
			WFCIntVector LightGridPos = LightedBlocksGridPos[j];

			if (BlockGridPos == LightGridPos)
			{
				SearchFlag = true;
				break;
			}
		}

		if (!SearchFlag)
		{
			//			AllBlock[i]->SetActorHiddenInGame(true);
			AllBlock[i]->Destroy();
		}
		else
		{
			TempArray.Emplace(AllBlock[i]);
		}
	}
	AllBlock.Empty();
	for (int i = 0; i < TempArray.Num(); i++)
	{
		AllBlock.Emplace(TempArray[i]);
	}
}

void AWFCArea::AddNewBlocks()
{
	for (int i = 0; i < LightedBlocksGridPos.Num(); i++)
	{
		bool SearchFlag = false;
		for (int j = 0; j < AllBlock.Num(); j++)
		{
			WFCIntVector LightGridPos = LightedBlocksGridPos[i];
			WFCIntVector BlockGridPos = AllBlock[j]->GridPos;

			if (BlockGridPos == LightGridPos)
			{
				SearchFlag = true;
				break;
			}
		}

		if (!SearchFlag)
		{
			NeedToBlockGridPos.Emplace(LightedBlocksGridPos[i]);
		}
	}
	for (int i = 0; i < NeedToBlockGridPos.Num(); i++)
	{
		GenerateBlockAtPos(NeedToBlockGridPos[i]);
	}
}

void AWFCArea::GenerateBlockAtPos(const WFCIntVector& Pos)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		AWFCBlock* block;
		int rand = WFCUtil::getRandom(0, 1);
		if (rand == 0)
		{
			block = World->SpawnActor<AWFCBlock>(BlockClass1, FVector(0, 0, 0), FRotator(0, 0, 0));
		}
		else
		{
			block = World->SpawnActor<AWFCBlock>(BlockClass2, FVector(0, 0, 0), FRotator(0, 0, 0));
		}
		AllBlock.Emplace(block);

		block->SetActorLocation(FVector(Pos.x*GridSize, Pos.y*GridSize, Pos.z*GridSize));
		block->GridPos = Pos;
	}
}