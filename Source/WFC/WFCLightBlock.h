// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <vector>

#include "WFCBlock.h"
#include "WFCIntVector.h"
#include "WFCBlock.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "GameFramework/Pawn.h"
#include "WFCGrid.h"
#include "WFCOrientation.h"

#include "WFCLightBlock.generated.h"




//模拟的球形点亮区域
UCLASS()
class WFC_API AWFCLightBlock : public AWFCBlock
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFCLightBlock();

	void SetLightSize(int Size);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Light")
	int LightValue;

	//可观测半径
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Light")
	int LightSize;
public:	
	virtual void Tick(float DeltaTime) override;

	//TODO
	//由光源位置计算光照区域
	//考虑遮挡
	//种子填充？
	void UpdateLightedBlocksGridPos();

	void ActionTurn();

	TArray<FWFCIntVector> LightedBlocksGridPos;
};
