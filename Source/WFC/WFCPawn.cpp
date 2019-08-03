// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCPawn.h"

FTest::FTest()
{
	UE_LOG(LogTemp, Warning, TEXT("1111FTest Constrction"));
}

FTest::~FTest()
{
	UE_LOG(LogTemp, Warning, TEXT("2222FTest Destrction"));
}


//////////////////////////////////////////////////////////////////////////

// Sets default values
AWFCPawn::AWFCPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWFCPawn::BeginPlay()
{
	Super::BeginPlay();

	Test = MakeShareable(new FTest());
	
}

// Called every frame
void AWFCPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWFCPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


