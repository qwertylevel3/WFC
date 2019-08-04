// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCGrid.h"
#include "WFCUtil.h"


WFCGrid::WFCGrid()
{
}

WFCGrid::WFCGrid(const WFCIntVector& Pos)
	:GridPos(Pos)
{
}

WFCGrid::~WFCGrid()
{
	auto iter = AllBlock.begin();
	while (iter!=AllBlock.end())
	{
		(*iter)->Destroy();
		iter++;
	}
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
	while (iter!=AllBlock.end())
	{
		if (!(*iter)->CanOverlap)
		{
			return false;
		}
		iter++;
	}
	return true;
}
