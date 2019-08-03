// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct WFC_API WFCIntVector
{
public:


	int x;
	int y;
	int z;
	WFCIntVector(int xx=0, int yy=0, int zz=0);
	void Reverse();

	int GetDistance(const WFCIntVector& b);

	bool operator==(const WFCIntVector& b)
	{
		return (this->x == b.x) && (this->y == b.y) && (this->z == b.z);
	}

	void operator=(const WFCIntVector& b)
	{
		x = b.x;
		y = b.y;
		z = b.z;
	}

	WFCIntVector operator+(const WFCIntVector& b)
	{
		WFCIntVector ivec;
		ivec.x = this->x + b.x;
		ivec.y = this->y + b.y;
		ivec.z = this->z + b.z;
		return ivec;
	}

	int GetVolume();
};
