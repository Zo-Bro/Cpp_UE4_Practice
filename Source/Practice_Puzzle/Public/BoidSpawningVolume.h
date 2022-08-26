// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "BoidSpawningVolume.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_PUZZLE_API ABoidSpawningVolume : public AVolume
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = Boids)
	class UMaterialInstance* Mat;

public:
	ABoidSpawningVolume();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABoid> Boid;

	UPROPERTY(EditAnywhere, Category = Boids)
		int NumberOfBoidsToSpawn;
	UPROPERTY(EditAnywhere, Category = Movement)
		FVector InitialVelocity;
	UPROPERTY(EditAnywhere, Category = Movement)
		FVector Velocity;
	UPROPERTY(EditAnywhere, Category = Movement)
		float ViewAngle;
	UPROPERTY(EditAnywhere, Category = Movement)
		float ViewDistance;
	UPROPERTY(EditAnywhere, Category = Movement)
		float MinSpeed;
	UPROPERTY(EditAnywhere, Category = Movement)
		float MaxSpeed;
	UPROPERTY(EditAnywhere, Category = Movement)
		float Handling;
	UPROPERTY(EditAnywhere, Category = Movement)
		float WanderIntensity;
	// the radius of the wander sphere
	UPROPERTY(EditAnywhere, Category = Movement)
		float WanderSphereSize;
	// How far away from the nose of the boid the wander sphere is
	UPROPERTY(EditAnywhere, Category = Movement)
		float WanderSphereProjectionDistance;

protected:
	virtual void BeginPlay() override;
};
