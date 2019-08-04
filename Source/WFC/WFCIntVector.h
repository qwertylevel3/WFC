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
	WFCIntVector(int xx = 0, int yy = 0, int zz = 0);
	void Reverse();

	int GetDistance(const WFCIntVector& b);
	double GetDistanceD(const WFCIntVector& b);

	bool operator==(const WFCIntVector& b)
	{
		return (this->x == b.x) && (this->y == b.y) && (this->z == b.z);
	}

	//拷贝构造函数
	WFCIntVector(const WFCIntVector &t) {
		x = t.x;
		y = t.y;
		z = t.z;
	}
	//重载=号运算符
	WFCIntVector& operator= (const WFCIntVector &t)
	{
		if (this != &t) {
			x = t.x;
			y = t.y;
			z = t.z;
		}
		return *this;
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
