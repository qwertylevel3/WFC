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
