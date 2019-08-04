// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCBlock.h"

// Sets default values
AWFCBlock::AWFCBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CanOverlap = false;
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

