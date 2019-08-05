// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WFCIntVector.h"
#include "GameFramework/Pawn.h"
#include "WFCOrientation.h"
//#include "GameFramework/Actor.h"
#include "WFCBlock.generated.h"

class AWFCPlayZone;

UCLASS()
class WFC_API AWFCBlock : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFCBlock();

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	bool CanOverlap;

	//可以移动到上面(上方是否为空另外检测)
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	bool CanStep;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Block")
	FWFCIntVector GridPos;

	UFUNCTION(BlueprintNativeEvent)
	void MoveAnimation(const FWFCIntVector& OriPos,const FWFCIntVector& TargetPos);

	AWFCPlayZone* PlayZone;

	bool ReadyToDead;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartTurn();
	virtual void ActionTurn();
	virtual void EndTurn();
};
