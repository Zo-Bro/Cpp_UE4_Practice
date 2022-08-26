// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "MathUtilities.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ABoid::ABoid()
{
	// A Struct for Storing UAssets 
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> TriangleBoid;
		FConstructorStatics()
			: TriangleBoid(TEXT("/Game/StarterContent/Shapes/Shape_TriPyramid.Shape_TriPyramid"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
 	
	Mat = nullptr;
	
	SkeletalMeshScale = 0.1f;
	InitialVelocity = FVector(20.0f, 0.0f, 0.0f);
	Velocity = InitialVelocity;
	ViewAngle = 120.0f;
	ViewDistance = 100.0f;
	Handling = 1.0f;
	WanderSphereSize = 20.0f;
	WanderSphereProjectionDistance = 100.0f;
	WanderIntensity = 10.0f;
	SteeringForce = FMath::VRand()*WanderSphereSize;
	

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMesh"));
	SkeletalMesh->SetRelativeScale3D(FVector(SkeletalMeshScale, SkeletalMeshScale, SkeletalMeshScale));
	SkeletalMesh->SetupAttachment(this->GetRootComponent());
	SkeletalMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	//This is now a point on a sphere 
	//SteeringForce = SteeringForce + (Velocity.Normalize()*WanderSphereProjectionDistance);
}


// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovement(DeltaTime);
}

void ABoid::Initialize(FVector StartingPosition, FVector StartingVelocity, float StartingSpeed,
	float InHandling, float InViewAngle, float InViewDistance, float InWanderSphereSize,
	float InWanderSphereProjectionDistance, float InWanderIntensity, 
	UMaterialInstance* InMaterial, int MovementMode, AVolume* InBoundingVolume)
{
	InitialVelocity = StartingVelocity;
	Velocity = InitialVelocity;
	ViewAngle = InViewAngle;
	ViewDistance = InViewDistance;
	Handling = InHandling;
	WanderSphereSize = InWanderSphereSize;
	WanderSphereProjectionDistance = InWanderSphereProjectionDistance;
	WanderIntensity =InWanderIntensity;
	SkeletalMesh->SetMaterial(0, UMaterialInstanceDynamic::Create(InMaterial, this, FName("MID_Boid")));
	this->SetActorLocation(StartingPosition);
	if (!StartingVelocity.IsNearlyZero())
	{
		this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(StartingPosition, StartingPosition + StartingVelocity));
		Speed = StartingSpeed;
		MaxSpeed = StartingSpeed;
		Velocity = StartingVelocity * Speed;
		// This is a point on a sphere of radius size WanderControl
		SteeringForce = FMath::VRand()*WanderSphereSize;
		//Push this sphere ahead of the boid by X units
		//SteeringForce = SteeringForce + (Velocity.Normalize()*WanderSphereProjectionDistance);

	}
	if (IsValid(InBoundingVolume))
	{
		BoundingVolume = InBoundingVolume;
	}
}

void ABoid::UpdateMovement(float DeltaTime)
{
	//ToDo:
	/*
		- 
	-Once Flocking Works, add ability to dodge projectiles/seek target
	*/
	FVector DeltaMovement = FVector::ZeroVector;
	FVector FinalPosition = FVector::ZeroVector;
	//calculate velocity first before applying it this frame
	// This is where "behavior" occurs
	bool bShouldFlock = false;
	TArray<AActor*> NearbyBoids = TArray<AActor*>();
	TArray<AActor*> ActorsToIgnore = TArray<AActor*>();
	ActorsToIgnore.Add(this);
	float DesireToWander;
	// check if we should flock

	if (UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), ViewDistance, TArray<TEnumAsByte<EObjectTypeQuery>>(), ABoid::StaticClass(), ActorsToIgnore, NearbyBoids))
	{
		DesireToWander = 1.5f;
		int FlockSize = 0;
		//Get the average Velocity of the flock
		FVector AvgVelocity = FVector::ZeroVector;
		FVector AvgPosition = FVector::ZeroVector;

		for (AActor* Boid : NearbyBoids)
		{
			ABoid* TempBoid = Cast<ABoid>(Boid);
			if (TempBoid)
			{
				//arccosine((vecA * vecB) / (vecA.Size() * vecB.Size()))
				FVector TempLoc = TempBoid->GetActorLocation();
				FVector MyLoc = this->GetActorLocation();
				FVector TempBoidLocRelativeToMe = FVector(TempLoc.X - MyLoc.X, TempLoc.Y - MyLoc.Y, TempLoc.Z - MyLoc.Z);
				float AngleBetweenMeAndOther = UMathUtilities::GetAngleBetweenVectors(Velocity, TempBoidLocRelativeToMe);
				//float AngleBetweenMeAndOther = FMath::Acos(FVector::DotProduct(Velocity, TempBoidLocRelativeToMe) / (Velocity.Size() * TempBoidLocRelativeToMe.Size()));
				//only add boid if the angle formed by my forward vector and the vector formed by my position to theirs is within threshold
				
				//ToDo: only try to sync up if the flock is moving in a "similar" direction as me.
				float DifferenceInDirection = UMathUtilities::GetAngleBetweenVectors(Velocity, TempBoid->Velocity);
				if (AngleBetweenMeAndOther <= 120.0f && DifferenceInDirection <= 45.0f)
				{
					AvgVelocity += TempBoid->Velocity;
					AvgPosition += TempBoid->GetActorLocation();
					FlockSize += 1;
				}

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Sphere Intersected Actor was not Boid. Oh Dear."));
			}
		}
		//Dont Forget yourself (need to check if sphere intersect will count myself)
		AvgVelocity = AvgVelocity + Velocity;
		AvgPosition = AvgPosition + GetActorLocation();
		AvgVelocity = AvgVelocity / (FlockSize + 1);
		AvgPosition = AvgPosition / (FlockSize + 1);

		// If Group velocity is too different from mine, adjust to group velocity.
		// This should happen the most. Only established flock members should not do this.
		if (UMathUtilities::GetAngleBetweenVectors(Velocity, AvgVelocity) >= 10.0f)
		{
			Velocity = FMath::VInterpTo(Velocity, AvgVelocity, DeltaTime, Handling);
			SkeletalMesh->SetVectorParameterValueOnMaterials(FName("Color"), FVector(0, 1, 0));
		}
		// If not, try to become the center of the group if I feel "far away"
		else if ((AvgPosition - this->GetActorLocation()).Size() > 50.0 + (15.0f * FlockSize))
		{
			Velocity = FMath::VInterpTo(Velocity, (AvgPosition - this->GetActorLocation()), DeltaTime, Handling);
			SkeletalMesh->SetVectorParameterValueOnMaterials(FName("Color"), FVector(1, 0, 0));
		}
		// Angle slightly towards the average position of the flock.
		//Add some Speed because we will tend to slow down when flocking
		if (Velocity.Size() < MaxSpeed)
		{
			float SpeedIncrement = 1.0f;
			// The (desired length / current length) * current vector produces a vector of the desired length.
			Velocity = FMath::Clamp(((Velocity.Size() + SpeedIncrement)/Velocity.Size()), 0.0f, MaxSpeed) * Velocity;
		}


	}
	// No nearby boids, so I should wander instead
	else
	{
		DesireToWander = 3.0f;
		SkeletalMesh->SetVectorParameterValueOnMaterials(FName("Color"), FVector(0, 0, 1));
	}

	//Wander Behavior:
	/*
	A steeringForce exists X units ahead of the nose of the boid. The steering force is a randomly selected point on a sphere.
	Each frame, that steering force is "nudged" by a small amount, and then the resulting direction is merged a bit with the final velocity.
	*/

	//To get a random point on the outside of a sphere, you can just get a random vector of a specific length
	//To move that point incrementally around the same sphere, you can just rotate it by 1 degree a random amount
	FVector TargetPoint = FMath::VRand()*WanderIntensity;
	//Swizzling the values unintentionally (its random so it doesnt matter
	FRotator RandomRotator = FRotator(TargetPoint.X, TargetPoint.Y, TargetPoint.Z);
	SteeringForce = RandomRotator.RotateVector(SteeringForce);

	FVector WanderVelocity = SteeringForce + Velocity;
	WanderVelocity = (Speed / WanderVelocity.Size()) * WanderVelocity;
	//FVector WanderVelocity = (Velocity.Size() / (Velocity + SteeringForce).Size()) * (Velocity + SteeringForce);
	Velocity = FMath::VInterpTo(Velocity, WanderVelocity, DeltaTime, DesireToWander);


	//APPLY VELOCITY (ACTUALLY MOVE THE ACTOR)
	//Calculate Final Position
	DeltaMovement = Velocity * DeltaTime;
	FinalPosition = this->GetActorLocation() + DeltaMovement;
	//Set Rotation
	this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), FinalPosition));
	//Check if I need to be warped because my final position is out of bounds
	//This will warp across the origin, which looks kind of weird
	// would be nice to warp across perpendicular to the plane you collided with
	FVector OffsetToApply = FVector::ZeroVector;
	if (BoundingVolume && !BoundingVolume->EncompassesPoint(FinalPosition))
	{
		OffsetToApply = BoundingVolume->GetActorLocation() - FinalPosition;
		// use 1.9 instead of 2 so we dont accidentally warp out of bounds
		OffsetToApply = OffsetToApply * 1.9f;
	}
	//Then move to new position
	this->SetActorLocation(FinalPosition + OffsetToApply);
}