// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sphere_Grid_Manager.generated.h"
/*
USTRUCT() 
struct FSphereGridMember
{
	GENERATED_BODY()

	UPROPERTY()
		UStaticMeshComponent* SphereMesh;
	UPROPERTY()
		int xPos;
	UPROPERTY()
		int yPos;
	UPROPERTY()
		int zPos;
	UPROPERTY()
		FVector velocity;
	UPROPERTY()
		FVector accel;
	UPROPERTY()
		FVector startingLocation;
	UPROPERTY()
		//declare pointer to a float.
		float delay;

	FSphereGridMember()
	{
		SphereMesh = nullptr;
		xPos = 0; yPos = 0; zPos = 0; 
		velocity = FVector::ZeroVector; 
		accel = FVector::ZeroVector;
		startingLocation = FVector::ZeroVector;
		//set the value of the float that is pointed at by delay.
		delay = 0.0f;
		
	}

	FSphereGridMember(UStaticMeshComponent* InMeshComp, int inX, int inY, int inZ)
	{
		if (IsValid(InMeshComp))
		{
			SphereMesh = InMeshComp;
			startingLocation = SphereMesh->GetRelativeLocation();
		}
		else
		{
			SphereMesh = nullptr;
			startingLocation = FVector::ZeroVector;
			UE_LOG(LogCore, Warning, TEXT("Supplied StaticMesh was invalid."))
		}
		xPos = inX; yPos = inY; zPos = inZ;
		velocity = FVector::ZeroVector;
		accel = FVector::ZeroVector;
		delay = 0.0f;

	}
};
*/
UCLASS()
class PRACTICE_PUZZLE_API USphereGridMember : public UObject
{
public:
	GENERATED_BODY()
		/*
		A sphere mesh in a 3d grid that knows its position in the grid
		*/
		UPROPERTY()
		UStaticMeshComponent* SphereMesh;
	UPROPERTY()
		int xPos;
	UPROPERTY()
		int yPos;
	UPROPERTY()
		int zPos;
	UPROPERTY()
		FVector velocity;
	UPROPERTY()
		FVector accel;
	UPROPERTY()
		FVector startingLocation;
	UPROPERTY()
		//declare pointer to a float.
		float delay;
public:
	USphereGridMember();
public:
	USphereGridMember(UStaticMeshComponent* InMeshComp, int inX, int inY, int inZ);
};

UENUM(BlueprintType)
enum class ESphereGridMoveType : uint8 {
	SineWave       UMETA(DisplayName = "SineWave"),
	BoidAvoid	UMETA(DisplayName = "BoidAvoid"),
};

UCLASS()
class PRACTICE_PUZZLE_API ASphere_Grid_Manager : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	class USceneComponent* Root;
	
	UPROPERTY()
	class USceneComponent* Locator;

public:	
	// Sets default values for this actor's properties
	ASphere_Grid_Manager();

	UPROPERTY(EditAnywhere, Category= "Grid Size", Meta=(ClampMin=2, ClampMax=7))
		int Size = 4;
	UPROPERTY(VisibleAnywhere, Category = "Grid Size")
		int MaxSize = 7;

	UPROPERTY(EditAnywhere, Category = "Sphere Setup")
	UMaterialInstance* SphereMat;
	
	UPROPERTY(EditAnywhere, Category = "Sphere Setup")
	UStaticMesh* SphereMesh;

	UPROPERTY(EditAnywhere, Category = "Sphere Setup")
	float Spacing = 75.0f;

	// default sphere mesh is about 50 units diameter
	UPROPERTY(EditAnywhere, Category = "Sphere Setup")
	float Scale = 1.0f;

private:

	UPROPERTY()
	TMap<FVector, USphereGridMember*> SphereGridMap;

	UPROPERTY()
	TArray<USphereGridMember*> SphereGridArray;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	ESphereGridMoveType MovementProtocol;

	UPROPERTY(EditAnywhere, Category = "Movement - Sine", Meta=(EditCondition="MovementProtocol == ESphereGridMoveType::SineWave"))
	FVector SinePoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Movement - Sine")
		float Amplitude = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Movement - BoidAvoid")
		float ReturnHomeDelay = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Movement - BoidAvoid")
		float ViewDistance = 50.0f;

	UPROPERTY()
	UMaterialInstanceDynamic* SphereGridMID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	void GenerateSphereGrid(int xSize, int ySize, int zSize);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
