// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <list>
#include <unordered_map>
#include "WFCLightBlock.h"

#include "WFCPlayZone.generated.h"

struct NeedToBlockPos 
{
	//λ��
	FWFCIntVector Pos;
	//����ֵ��Խ���������ȼ�Խ��
	double LightValueD;
	int LightValue;
};

//���𳡾���block���ɺ͹���
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

	//player���λ�����Ƿ�����ص�block
	UFUNCTION(BlueprintCallable)
	bool CanOverlap(int x,int y,int z);

	//��ȡ���ƶ���������λ��(��������,��Ծ),-1-1-1��ʾ���ɵ���
	UFUNCTION(BlueprintCallable)
	FWFCIntVector GetMovealbePos(const FWFCIntVector& TargetPos);

//	UFUNCTION(BlueprintCallable)
//	FWFCIntVector GetMovealbePosByPlayerOrientation(EWFCOrientation Ori);


	void AddLightAtGridPos(AWFCLightBlock* Light,const FWFCIntVector& Pos);


	void NextTurn();

	void StartTurn();
	void ActionTurn();
	void EndTurn();

	std::unordered_map<std::string, GridPtr>& GetAllBlockGridRef();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void initGame();

	//������block����
	std::unordered_map<std::string, GridPtr> AllBlockGrid;
	//������block����
	std::list<NeedToBlockPos> NeedToBlockGridPosQueue;
	std::list<AWFCLightBlock*> AllLight;


	//�غ�ϵͳ�е�Pawn(������player)
	std::list<AWFCBlock*> PlayPawns;

	AWFCBlock* Player;

	int GridSize;
public:	

//	void UpdateAllLight();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AWFCBlock* GenerateBlockAtPos(UClass* Class,const FWFCIntVector& Pos);

	//ĳ��λ���Ƿ�����
	bool IsInLight(const FWFCIntVector& Pos);

	void AddBlockAtPos(AWFCBlock* Block, const FWFCIntVector& Pos);

//	//��������
//	void UpdateZone();

	//����ڹ۲ⷶΧ���blocks
	void RemoveOutBlocks();
	//������Ҫ��Gridλ�õ������� 
	void LightGridPos(int LightValue,const FWFCIntVector& LightPos, const FWFCIntVector& Pos);

	//�����Ĺ���
	void GenerateBlockToScene();
};
