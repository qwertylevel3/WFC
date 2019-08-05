// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCGrid.h"
#include "WFCUtil.h"

WFCGrid::WFCGrid()
{
}

WFCGrid::WFCGrid(const FWFCIntVector& Pos)
	:GridPos(Pos)
{
	LightValue = 3;
}

WFCGrid::~WFCGrid()
{
}

void WFCGrid::AddBlock(AWFCBlock* Block)
{
	AllBlock.push_back(Block);
}

std::string WFCGrid::GetHash()
{
	return WFCUtil::hashPos(GridPos.x, GridPos.y, GridPos.z);
}

bool WFCGrid::CanOverlap()
{
	bool Result = true;
	auto iter = AllBlock.begin();
	while (iter != AllBlock.end())
	{
		if (!(*iter)->CanOverlap)
		{
			return false;
		}
		iter++;
	}
	return true;
}

bool WFCGrid::CanStep()
{
	bool Result = false;
	auto iter = AllBlock.begin();
	while (iter != AllBlock.end())
	{
		if ((*iter)->CanStep)
		{
			return true;
		}
		iter++;
	}
	return false;
}

void WFCGrid::ClearBlocks()
{
	auto iter = AllBlock.begin();
	while (iter != AllBlock.end())
	{
		(*iter)->Destroy();
		iter++;
	}
}

void WFCGrid::StartTurn()
{
	auto iter = AllBlock.begin();
	while (iter != AllBlock.end())
	{
		(*iter)->StartTurn();
		iter++;
	}
}

void WFCGrid::ActionTurn()
{
	auto iter = AllBlock.begin();
	while (iter != AllBlock.end())
	{
		(*iter)->ActionTurn();
		iter++;
	}


}

void WFCGrid::EndTurn()
{
	auto iter = AllBlock.begin();
	while (iter != AllBlock.end())
	{
		(*iter)->EndTurn();
		iter++;
	}
}

std::list<AWFCBlock*>& WFCGrid::GetAllBlockRef()
{
	return AllBlock;
}
