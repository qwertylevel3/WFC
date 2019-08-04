// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include "WFCIntVector.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct WFC_API FWFCIntVector
{
	GENERATED_USTRUCT_BODY()
public:
	FWFCIntVector(int xx = 0, int yy = 0, int zz = 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
	int x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
	int y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
	int z;


	void Reverse();

	std::string GetHash() const;

	int GetDistance(const FWFCIntVector& b) const;
	double GetDistanceD(const FWFCIntVector& b) const;

	bool operator==(const FWFCIntVector& b)
	{
		return (this->x == b.x) && (this->y == b.y) && (this->z == b.z);
	}

	//拷贝构造函数
	FWFCIntVector(const FWFCIntVector &t) {
		x = t.x;
		y = t.y;
		z = t.z;
	}
	//重载=号运算符
	FWFCIntVector& operator= (const FWFCIntVector &t)
	{
		if (this != &t) {
			x = t.x;
			y = t.y;
			z = t.z;
		}
		return *this;
	}


	FWFCIntVector operator+(const FWFCIntVector& b)
	{
		FWFCIntVector ivec;
		ivec.x = this->x + b.x;
		ivec.y = this->y + b.y;
		ivec.z = this->z + b.z;
		return ivec;
	}

	int GetVolume() const;
};
