// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WFCBlock.h"
#include "WFCIntVector.h"

#include <list>
#include <string>
#include <memory>


/**
 * 
 */
class WFC_API WFCGrid
{
public:
	WFCGrid();
	~WFCGrid();
	WFCGrid(const FWFCIntVector& Pos);

	void AddBlock(AWFCBlock* Block);

	std::string GetHash();

	bool CanOverlap();
	bool CanStep();

	void ClearBlocks();

	void StartTurn();
	void ActionTurn();
	void EndTurn();

	FWFCIntVector GridPos;
	std::list<AWFCBlock*>& GetAllBlockRef();


	int LightValue;
protected:
	std::list<AWFCBlock*> AllBlock;
};

typedef std::shared_ptr<WFCGrid> GridPtr;
