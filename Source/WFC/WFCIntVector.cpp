// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCIntVector.h"
#include "WFCUtil.h"

WFCIntVector::WFCIntVector(int xx/*=0*/, int yy/*=0*/, int zz/*=0*/)
	:x(xx), y(yy), z(zz)
{
}

void WFCIntVector::Reverse()
{
	x *= -1;
	y *= -1;
	z *= -1;
}

std::string WFCIntVector::GetHash() const
{
	return WFCUtil::hashPos(x, y, z);
}

int WFCIntVector::GetDistance(const WFCIntVector& b) const
{
	int deltaX = b.x - this->x;
	int deltaY = b.y - this->y;
	int deltaZ = b.z - this->z;
	return sqrt(deltaX*deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

double WFCIntVector::GetDistanceD(const WFCIntVector& b) const
{
	double  deltaX = b.x - this->x;
	double  deltaY = b.y - this->y;
	double  deltaZ = b.z - this->z;
	return sqrt(deltaX*deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

int WFCIntVector::GetVolume() const
{
	return x * y*z;
}