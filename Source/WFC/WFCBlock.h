// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WFCIntVector.h"
#include "GameFramework/Pawn.h"
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

	//�����ƶ�������(�Ϸ��Ƿ�Ϊ��������)
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	bool CanStep;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Block")
	FWFCIntVector GridPos;

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
