// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCLightBlock.h"

#include "WFCUtil.h"
#include <DrawDebugHelpers.h>
#include <string>

#include "WFCPlayZone.h"

// Sets default values
AWFCLightBlock::AWFCLightBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightSize = 3;
	LightValue = 3;

	CanOverlap = true;
}

void AWFCLightBlock::SetLightSize(int Size)
{
	LightSize = Size;
	UpdateLightedBlocksGridPos();
}

// Called when the game starts or when spawned
void AWFCLightBlock::BeginPlay()
{
	Super::BeginPlay();

}

//	void AWFCLightArea::MoveToDirection(FWFCIntVector Direction)
//	{
//		GridPos = GridPos + Direction;
//	
//		//	DrawDebugSphere(GetWorld(), FVector(PlayerPos.x*GridSize,PlayerPos.y*GridSize,PlayerPos.z*GridSize), LightSize*GridSize, 12, FColor::Red, false, 2.0f, 0, 1.0f);
//	
//		UpdateLightedBlocksGridPos();
//		RemoveOutBlocks();
//		AddNewGridPos();
//	
//		//
//		//
//		//	for (int i=0;i<AllBlock.Num();i++)
//		//	{
//		//		WFCIntVec BlockIndex = GetBlockIndex(i);
//		//		WFCIntVec Pos = GetReletivePos(BlockIndex.x, BlockIndex.y, BlockIndex.z);
//		//		Pos = Pos + Direction;
//		//		TWeakObjectPtr<AWFCBlock> Block = AllBlock[i];
//		//		Block->SetActorLocation(FVector(Pos.x*GridSize, Pos.y*GridSize, Pos.z*GridSize));
//		//	}
//	}
//	
//	void AWFCLightArea::MoveForward()
//	{
//		MoveToDirection(FWFCIntVector(-1, 0, 0));
//	}
//	
//	void AWFCLightArea::MoveBack()
//	{
//		MoveToDirection(FWFCIntVector(1, 0, 0));
//	}
//	
//	void AWFCLightArea::MoveRight()
//	{
//		MoveToDirection(FWFCIntVector(0, -1, 0));
//	}
//	
//	void AWFCLightArea::MoveLeft()
//	{
//		MoveToDirection(FWFCIntVector(0, 1, 0));
//	}
//	
//	void AWFCLightArea::MoveUp()
//	{
//		MoveToDirection(FWFCIntVector(0, 0, 1));
//	}
//	
//	void AWFCLightArea::MoveDown()
//	{
//		MoveToDirection(FWFCIntVector(0, 0, -1));
//	}

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

//int AWFCLightArea::GetReletiveIndex(int Index, int PlayerIndex, int Length)
//{
//	int delta = Index - PlayerIndex;
//	int sign = delta < 0 ? -1 : 1;
//
//	if (abs(delta) <= 5)
//	{
//		return delta;
//	}
//	else
//	{
//		return (sign)*(Length - abs(delta));
//	}
//}

// Called every frame
void AWFCLightBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWFCLightBlock::UpdateLightedBlocksGridPos()
{
	LightedBlocksGridPos.Empty();

	for (int i = -LightSize; i <= LightSize; i++)
	{
		for (int j = -LightSize; j <= LightSize; j++)
		{
			for (int k = -LightSize; k <= LightSize; k++)
			{
				FWFCIntVector Pos(GridPos.x + i, GridPos.y + j, GridPos.z + k);

				if (GridPos.GetDistance(Pos) <= LightSize)
				{
					LightedBlocksGridPos.Emplace(Pos);
				}
			}
		}
	}
}

void AWFCLightBlock::ActionTurn()
{
	UpdateLightedBlocksGridPos();
	if (PlayZone)
	{
		for (int i=0;i<LightedBlocksGridPos.Num();i++)
		{
			PlayZone->LightGridPos(LightValue,GridPos,LightedBlocksGridPos[i]);
		}
	}
}
