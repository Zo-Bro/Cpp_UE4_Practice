// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class PRACTICE_PUZZLE_API ABoid : public AActor
{
	GENERATED_BODY()
		
		UPROPERTY(EditDefaultsOnly, Category=Rendering)
		class UStaticMeshComponent* Mesh;
		AVolume* BoundingVolume;
	// an actor
	//UPROPERTY(EditDefaultsOnly, Category = Movement)
	//	class AActor* Bounds;
public:	
	// Sets default values for this actor's properties
	ABoid();

private:
	UPROPERTY(EditAnywhere, Category = Movement)
		FVector InitialVelocity;
	UPROPERTY(EditAnywhere, Category = Movement)
		FVector Velocity;
	UPROPERTY(EditAnywhere, Category = Movement)
		float ViewAngle;
	UPROPERTY(EditAnywhere, Category = Movement)
		float ViewDistance;
	UPROPERTY(EditAnywhere, Category = Movement)
		float Speed;
	UPROPERTY(EditAnywhere, Category = Movement)
		float MaxSpeed;
	UPROPERTY(EditAnywhere, Category = Movement)
		float Handling;
	UPROPERTY(EditAnywhere, Category = Movement)
		float WanderIntensity;

	UPROPERTY(EditAnywhere)
		UMaterialInstanceDynamic* Mat;
	// A point on a sphere projected ahead of the boid that influences the boids movement
	UPROPERTY()
		FVector SteeringForce;
	// the radius of the wander sphere
	UPROPERTY(EditAnywhere, Category = Movement)
		float WanderSphereSize;
	// How far away from the nose of the boid the wander sphere is
	UPROPERTY(EditAnywhere, Category = Movement)
		float WanderSphereProjectionDistance;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Initialize(FVector StartingPosition, FVector StartingVelocity, float StartingSpeed,
		float InHandling, float InViewAngle, float InViewDistance, float InWanderSphereSize,
		float InWanderSphereProjectionDistance, float InWanderIntensity,
		UMaterialInstance* InMaterial, int MovementMode, AVolume* InBoundingVolume);

private:

	UFUNCTION()
		virtual void UpdateMovement(float DeltaTime);
};
