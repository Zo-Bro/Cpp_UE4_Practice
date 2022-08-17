// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathUtilities.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_PUZZLE_API UMathUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
		UFUNCTION()
		static float GetAngleBetweenVectors(FVector Vector1, FVector Vector2);
	
};
