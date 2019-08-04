// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <string>

/**
 * 
 */
class WFC_API WFCUtil
{
public:
	WFCUtil();
	~WFCUtil();

	static int getRandom(int a, int b);
	static std::string hashPos(int x, int y, int z);
};
