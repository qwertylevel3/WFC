// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WFCIntVector.h"
#include "GameFramework/Actor.h"
#include "WFCBlock.generated.h"

UCLASS()
class WFC_API AWFCBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFCBlock();

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	bool CanOverlap;

	WFCIntVector GridPos;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
