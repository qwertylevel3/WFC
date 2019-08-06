// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <list>
#include <unordered_map>
#include "WFCLightBlock.h"

#include "WFCPlayZone.generated.h"

struct NeedToGridPos 
{
	//位置
	FWFCIntVector Pos;
	//亮度值，越大生成优先级越高
	double LightValueD;
	int LightValue;
};

//负责场景中block生成和管理
UCLASS()
class WFC_API AWFCPlayZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFCPlayZone();

	UPROPERTY(EditDefaultsOnly, Category=Block)
	TSubclassOf<class AWFCBlock> BlockClass1;

	UPROPERTY(EditDefaultsOnly, Category=Block)
	TSubclassOf<class AWFCBlock> BlockClass2;

	UPROPERTY(EditDefaultsOnly, Category=Block)
	TSubclassOf<class AWFCBlock> PlayerClass;

	//player相对位置上是否可以重叠block
	UFUNCTION(BlueprintCallable)
	bool CanOverlap(int x,int y,int z);

	//获取可移动到的最终位置(可能落下,跳跃)
	UFUNCTION(BlueprintCallable)
	FWFCIntVector GetMovealbePos(const FWFCIntVector& OriPos,const FWFCIntVector& TargetPos);

	UFUNCTION(BlueprintCallable)
	FWFCIntVector GetMovealbePosByDirection(const FWFCIntVector& OriPos,EWFCOrientation Direction);






	UFUNCTION(BlueprintCallable)
	void NextTurn();

	UFUNCTION(BlueprintCallable)
	void MoveBlock(AWFCBlock* Block, const FWFCIntVector& OriPos, const FWFCIntVector& TargetPos);

	UFUNCTION(BlueprintCallable)
	void TryMoveBlockByDirection(AWFCBlock* Block, const FWFCIntVector& OriPos,EWFCOrientation Direction);


	std::unordered_map<std::string, GridPtr>& GetAllBlockGridRef();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void initGame();

	//场景中block区域
	std::unordered_map<std::string, GridPtr> AllBlockGrid;
	//待生成block队列
	std::list<NeedToGridPos> NeedToGridPosQueue;
	std::list<AWFCLightBlock*> AllLight;


	//回合系统中的Pawn(不包括player)
	std::list<AWFCBlock*> PlayPawns;

	AWFCBlock* Player;

	int GridSize;
public:	

//	void UpdateAllLight();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AWFCBlock* GenerateBlockAtPos(UClass* Class,const FWFCIntVector& Pos);

	void GenerateGridAtPos(NeedToGridPos PosInfo);

	void CheckLight();

	void AddBlockAtPos(AWFCBlock* Block, const FWFCIntVector& Pos);
	void AddBlockAtPosWithoutWorldLocation(AWFCBlock* Block, const FWFCIntVector& Pos);

//	//更新区域
//	void UpdateZone();

	//清除在观测范围外的blocks
	void RemoveOutBlocks();
	//新增需要的Grid位置到队列中 
	void LightGridPos(int LightValue,const FWFCIntVector& LightPos, const FWFCIntVector& Pos);

	//真正的构造
	void GenerateGridToScene();
};
