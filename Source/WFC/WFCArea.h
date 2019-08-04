// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <vector>

#include <list>
#include "WFCIntVector.h"
#include "WFCBlock.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "GameFramework/Pawn.h"
#include "WFCGrid.h"
#include <unordered_map>

#include "WFCArea.generated.h"



UENUM(BlueprintType)
enum class EWFCOrientation : uint8
{
	E_UP   UMETA(DisplayName = "Up"),
	E_DOWN   UMETA(DisplayName = "Down"),
	E_LEFT   UMETA(DisplayName = "Left"),
	E_RIGHT   UMETA(DisplayName = "Right")
};

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


	//player相对位置上是否可以重叠block
	UFUNCTION(BlueprintCallable)
	bool CanOverlap(int x,int y,int z);



	//获取可移动到的最终位置(可能落下,跳跃),-1-1-1表示不可到达
	UFUNCTION(BlueprintCallable)
	FWFCIntVector GetMovealbePos(const FWFCIntVector& TargetPos);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void initArea();
public:	


	void MoveToDirection(FWFCIntVector Direction);

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

	//获取Block的相对于player的位置
//	WFCIntVector GetReletivePos(int x,int y,int z);

	int GetReletiveIndex(int Index,int PlayerIndex,int Length);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GridSize;


	void UpdateLightedBlocksGridPos();

	//清除在观测范围外的blocks
	void RemoveOutBlocks();

	//新增需要的Grid位置到队列中 
	void AddNewGridPos();
	//真正的构造
	void AddBlockToScene();




	//玩家Grid位置
	FWFCIntVector PlayerPos;
	//可观测半径
	int LightSize;

	TArray<FWFCIntVector> LightedBlocksGridPos;

	//已经在场景中的Block
//	TArray<TWeakObjectPtr<WFCGrid> > AllBlock;
	
	std::unordered_map<std::string, GridPtr> AllBlockGrid;

	std::list<FWFCIntVector> NeedToBlockGridPos;

	void GenerateBlockAtPos(const FWFCIntVector& Pos);
	void AddBlockAtPos(AWFCBlock* Block, const FWFCIntVector& Pos);
};
