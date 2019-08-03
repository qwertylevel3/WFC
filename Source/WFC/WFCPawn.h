// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridBase.h"


#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WFCPawn.generated.h"



class FTest
{
public:
	FTest();
	~FTest();
};

UCLASS()
class WFC_API AWFCPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWFCPawn();


    TSharedPtr<FTest> Test;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
