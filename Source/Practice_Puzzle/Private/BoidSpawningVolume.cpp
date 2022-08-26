// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidSpawningVolume.h"
#include "Boid.h"
#include "Kismet/KismetMathLibrary.h"

ABoidSpawningVolume::ABoidSpawningVolume()
{
	Mat = nullptr;
	NumberOfBoidsToSpawn = 20;
	MinSpeed = 80.0f;
	MaxSpeed = 120.0f;
	InitialVelocity = FVector(20.0f, 0.0f, 0.0f);
	Velocity = InitialVelocity;
	ViewAngle = 120.0f;
	ViewDistance = 100.0f;
	Handling = 1.0f;
	WanderSphereSize = 20.0f;
	WanderSphereProjectionDistance = 100.0f;
	WanderIntensity = 10.0f;
}

void ABoidSpawningVolume::BeginPlay()
{
	FBoxSphereBounds MyBounds = GetBounds();
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (Boid)
	{
		for (int i = 0; i < NumberOfBoidsToSpawn; i++)
		{
			FVector RandLocation = FVector::ZeroVector;
			RandLocation = UKismetMathLibrary::RandomPointInBoundingBox(MyBounds.Origin, MyBounds.BoxExtent);

			ABoid* TempBoid = GetWorld()->SpawnActor<ABoid>(Boid, SpawnInfo);
			float NewSpeed = FMath::RandRange(MinSpeed, MaxSpeed);
			FVector NewVelocity = UKismetMathLibrary::RandomUnitVector();

			TempBoid->Initialize(RandLocation, NewVelocity, NewSpeed,
				Handling, ViewAngle, ViewDistance, WanderSphereSize,
				WanderSphereProjectionDistance, WanderIntensity,
				Mat, 0, this);
		}
	}
}