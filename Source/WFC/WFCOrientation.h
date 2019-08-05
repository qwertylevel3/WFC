// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWFCOrientation : uint8
{
	E_UP   UMETA(DisplayName = "Up"),
	E_DOWN   UMETA(DisplayName = "Down"),
	E_LEFT   UMETA(DisplayName = "Left"),
	E_RIGHT   UMETA(DisplayName = "Right")
};
