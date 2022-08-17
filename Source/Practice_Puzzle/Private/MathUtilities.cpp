// Fill out your copyright notice in the Description page of Project Settings.


#include "MathUtilities.h"

//Returns the angle formed by two vectors
float UMathUtilities::GetAngleBetweenVectors(FVector Vector1, FVector Vector2)
{
	return (180 * (FMath::Acos(FVector::DotProduct(Vector1, Vector2) / (Vector1.Size() * Vector2.Size())))) / PI;
}
