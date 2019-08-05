// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCBlock.h"
#include "WFCPlayZone.h"
#include "Macro.h"

// Sets default values
AWFCBlock::AWFCBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CanOverlap = false;
	CanStep = false;

	PlayZone = nullptr;
	ReadyToDead = false;
}



void AWFCBlock::MoveAnimation_Implementation(const FWFCIntVector& OriPos, const FWFCIntVector& TargetPos)
{
	SetActorLocation(FVector(TargetPos.x*GRID_SIZE, TargetPos.y*GRID_SIZE, TargetPos.z*GRID_SIZE));
}

// Called when the game starts or when spawned
void AWFCBlock::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWFCBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWFCBlock::StartTurn()
{
}

void AWFCBlock::ActionTurn()
{
}

void AWFCBlock::EndTurn()
{
}