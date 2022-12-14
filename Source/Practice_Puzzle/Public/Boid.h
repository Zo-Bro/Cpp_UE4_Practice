// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

//ToDo: swap out static mesh for a skeletal mesh, and try to get all the boids to be birds!

UCLASS()
class PRACTICE_PUZZLE_API UBoidMovementComponent : public UObject
{
public:
	GENERATED_BODY()
		/*
		A grouping of variables that are needed by the boid to fly
		*/

		//Wing Flap
		UPROPERTY()
		bool bShouldFlap;
		UPROPERTY()
		float desireToFlap;
		//How much to increase flap desire per second
		UPROPERTY()
		float flapIncrement;
		UPROPERTY()
		float flapForce;
		UPROPERTY()
		float flapTimeoutLength;
		UPROPERTY()
		bool bCanFlap;
		UPROPERTY()
		FTimerHandle flapTimerHandle;
public:
	UBoidMovementComponent();

public:
	void IncrementWantToFlap();
	
	void Flap();
	
	void EndFlapTimeout();
};



UCLASS()
class PRACTICE_PUZZLE_API ABoid : public AActor
{
	GENERATED_BODY()
		
		UPROPERTY(EditDefaultsOnly, Category=Character)
		class USkeletalMeshComponent* SkeletalMesh;

		AVolume* BoundingVolume;
public:	
	// Sets default values for this actor's properties
	ABoid();

private:
	UPROPERTY(EditDefaultsOnly, Category = Character)
		float SkeletalMeshScale;
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

	//Animation
	UPROPERTY()
	UBoidMovementComponent* MovementComponent;

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

public:
	//Animation Events
	UFUNCTION(BlueprintCallable)
	UBoidMovementComponent* GetMovementComponent() {return MovementComponent; }
	
	UFUNCTION(BlueprintImplementableEvent)
	void Flap();


};
