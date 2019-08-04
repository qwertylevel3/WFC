// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCUtil.h"

WFCUtil::WFCUtil()
{
}

WFCUtil::~WFCUtil()
{
}

int WFCUtil::getRandom(int a, int b)
{
	return (rand() % (b - a + 1)) + a;
}

std::string WFCUtil::hashPos(int x, int y, int z)
{
	return std::string(std::to_string(x) + std::to_string(y) + std::to_string(z));
}
