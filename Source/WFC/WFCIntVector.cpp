// Fill out your copyright notice in the Description page of Project Settings.

#include "WFCIntVector.h"

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

int WFCIntVector::GetDistance(const WFCIntVector& b)
{
	int deltaX = b.x - this->x;
	int deltaY = b.y - this->y;
	int deltaZ = b.z - this->z;
	return sqrt(deltaX*deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

int WFCIntVector::GetVolume()
{
	return x * y*z;
}