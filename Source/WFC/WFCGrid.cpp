// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCGrid.h"
#include "WFCUtil.h"

WFCGrid::WFCGrid()
{
}

WFCGrid::WFCGrid(const FWFCIntVector& Pos)
	:GridPos(Pos)
{
	LightValue = 0;
}

WFCGrid::~WFCGrid()
{
}

void WFCGrid::AddBlock(AWFCBlock* Block)
{
	AllBlock.push_back(Block);
}

void WFCGrid::RemoveBlock(AWFCBlock* Block)
{
	auto iter = AllBlock.begin();
	while (iter!=AllBlock.end())
	{
		if ((*iter)==Block)
		{
			iter=AllBlock.erase(iter);
			return;
		}
		iter++;
	}
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

void WFCGrid::CheckLight()
{
	LightValue--;
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

std::list<AWFCBlock*>& WFCGrid::GetAllBlockRef()
{
	return AllBlock;
}
