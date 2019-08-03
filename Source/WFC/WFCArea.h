// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <vector>

#include "WFCIntVector.h"
#include "WFCBlock.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "GameFramework/Pawn.h"
#include "WFCArea.generated.h"


UCLASS()
class WFC_API AWFCArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFCArea();

	UPROPERTY(EditDefaultsOnly, Category=Block)
	TSubclassOf<class AWFCBlock> BlockClass1;

	UPROPERTY(EditDefaultsOnly, Category=Block)
	TSubclassOf<class AWFCBlock> BlockClass2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void initArea();
public:	


	void MoveToDirection(WFCIntVector Direction);

	UFUNCTION(BlueprintCallable)
	void MoveForward();
	UFUNCTION(BlueprintCallable)
	void MoveBack();

	UFUNCTION(BlueprintCallable)
	void MoveRight();
	UFUNCTION(BlueprintCallable)
	void MoveLeft();

	UFUNCTION(BlueprintCallable)
	void MoveUp();
	UFUNCTION(BlueprintCallable)
	void MoveDown();

//	TWeakObjectPtr<AWFCBlock> GetBlock(int x, int y, int z);
//	WFCIntVector GetBlockIndex(int Index);
//
//	void UpdateBlockPosition();

	//��ȡBlock�������player��λ��
//	WFCIntVector GetReletivePos(int x,int y,int z);

	int GetReletiveIndex(int Index,int PlayerIndex,int Length);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GridSize;


	void UpdateLightedBlocksGridPos();

	//����ڹ۲ⷶΧ���blocks
	void RemoveOutBlocks();

	//������Ҫ��blocks
	void AddNewBlocks();

	//���Gridλ��
	WFCIntVector PlayerPos;
	//�ɹ۲�뾶
	int LightSize;

	TArray<WFCIntVector> LightedBlocksGridPos;

	//�Ѿ��ڳ����е�Block
	TArray<TWeakObjectPtr<AWFCBlock> > AllBlock;

	TArray<WFCIntVector> NeedToBlockGridPos;


	void GenerateBlockAtPos(const WFCIntVector& Pos);
};
