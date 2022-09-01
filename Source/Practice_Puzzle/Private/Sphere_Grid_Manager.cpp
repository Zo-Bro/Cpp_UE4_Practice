// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize("", off)

#include "Sphere_Grid_Manager.h"

#include "Boid.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstance.h"


USphereGridMember::USphereGridMember()
{
	SphereMesh = nullptr;
	xPos = 0; yPos = 0; zPos = 0;
	velocity = FVector::ZeroVector;
	accel = FVector::ZeroVector;
	startingLocation = FVector::ZeroVector;
	//set the value of the float that is pointed at by delay.
	delay = 0.0f;

}
USphereGridMember::USphereGridMember(UStaticMeshComponent* InMeshComp, int inX, int inY, int inZ)
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
// Sets default values
ASphere_Grid_Manager::ASphere_Grid_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereMat = nullptr;
	SphereGridMID = nullptr;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	//Root->RegisterComponent();
	//this->AddInstanceComponent(Root);
	this->FinishAndRegisterComponent(Root);
	Locator = CreateDefaultSubobject<USceneComponent>(TEXT("Locator"));
	Locator->RegisterComponent();
	Locator->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	this->AddInstanceComponent(Locator);

}

// Called when the game starts or when spawned
void ASphere_Grid_Manager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASphere_Grid_Manager::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	SphereGridArray.Empty();
	SphereGridMap.Empty();
	if (Size <= MaxSize && IsValid(SphereMesh) && IsValid(SphereMat))
	{
		GenerateSphereGrid(Size, Size, Size);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Desired Grid Size is too large. Did not generate"))
	}
}

void ASphere_Grid_Manager::GenerateSphereGrid(int xSize, int ySize, int zSize)
{
	// Create MID 
	SphereGridMID = UMaterialInstanceDynamic::Create(SphereMat, this);
	SphereGridMID->SetScalarParameterValue(FName("GridSize"), Size);
	SphereGridMID->SetScalarParameterValue(FName("Spacing"), Spacing);
	SphereGridMID->SetVectorParameterValue(FName("LocatorPositionAbs"), Locator->GetComponentLocation());
	//TempMID->SetScalarParameterValue(FName("DistanceFromLocator"), 
	for (int x = 0; x < xSize ; x++ )
	{
		for (int y = 0; y < ySize; y++)
		{
			for (int z = 0; z < zSize; z++)
			{
				UStaticMeshComponent* TempSphere = NewObject<UStaticMeshComponent>(this);
				TempSphere->SetStaticMesh(SphereMesh);
				TempSphere->SetMobility(EComponentMobility::Movable);
				//TempSphere->SetupAttachment(Root);
				TempSphere->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
				TempSphere->RegisterComponent();
				this->AddInstanceComponent(TempSphere);
				TempSphere->CreationMethod = EComponentCreationMethod::UserConstructionScript;


				// Set MID Parameters Here

				TempSphere->SetMaterial(0, SphereGridMID);
				TempSphere->SetRelativeLocation(FVector(Spacing*x, Spacing*y, Spacing*z));


				//USphereGridMember SphereGridMember = USphereGridMember(TempSphere, x, y, z);
				USphereGridMember* SphereGridMember = NewObject<USphereGridMember>(this);
				SphereGridMember->SphereMesh = TempSphere;
				SphereGridMember->xPos = x;
				SphereGridMember->yPos = y;
				SphereGridMember->zPos = z;
				SphereGridMember->startingLocation = TempSphere->GetRelativeLocation();
				SphereGridMap.Add(FVector(x, y, z), SphereGridMember);
				SphereGridArray.Add(SphereGridMember);

			}
		}
	}
}

// Called every frame
void ASphere_Grid_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	for (USphereGridMember* Member : SphereGridArray)
	{
		FVector CachedLocation = FVector::ZeroVector;
		if (IsValid(Member->SphereMesh))
		{
			CachedLocation = Member->startingLocation;

			switch (MovementProtocol)
			{

			case ESphereGridMoveType::SineWave:
			{
				float Offset = 0.0f;
				// Sine Wave based movement here
				// Get vector pointing towards specific point (subtract start point from terminal point)
				Member->accel = FVector(Locator->GetRelativeLocation() - CachedLocation);
				Offset = FVector::Distance(Locator->GetRelativeLocation(), CachedLocation) / (Size*Spacing);
				Member->accel.Normalize();
				Member->accel = Member->accel * FMath::Sin((GetWorld()->GetRealTimeSeconds() * Speed) + Offset) * Amplitude;

				Member->SphereMesh->SetVectorParameterValueOnMaterials(FName("LocatorPositionAbs"), Locator->GetComponentLocation());
				Member->velocity = Member->accel * DeltaTime;
				Member->SphereMesh->SetRelativeLocation(Member->SphereMesh->GetRelativeLocation() + Member->velocity);
				break;
			}
			case ESphereGridMoveType::BoidAvoid:
			{
				TArray<AActor*> NearbyBoids = TArray<AActor*>();
				TArray<AActor*> ActorsToIgnore = TArray<AActor*>();
				ActorsToIgnore.Add(this);
				FVector AveragePosition = FVector::ZeroVector;
				FVector VecFromBoidsToSphereComp = FVector::ZeroVector;
				FVector FinalPosition = FVector::ZeroVector;
				float SphereDiameter = (Member->SphereMesh->GetRelativeScale3D().X *50.0f);

				// Note to Self: Should definitely have spawned actors instead of components. This relative to world location stuff is getting nasty
				if (UKismetSystemLibrary::SphereOverlapActors(this, Member->SphereMesh->GetRelativeLocation() + GetActorLocation(), SphereDiameter + ViewDistance, TArray<TEnumAsByte<EObjectTypeQuery>>(), ABoid::StaticClass(), ActorsToIgnore, NearbyBoids))
				{
					for (AActor* Boid : NearbyBoids)
					{
						AveragePosition += Boid->GetActorLocation();
					}
					AveragePosition = AveragePosition / NearbyBoids.Num(); // World Position
					// Get the vector that points away from the average position of boids
					VecFromBoidsToSphereComp = this->GetActorLocation() + Member->SphereMesh->GetRelativeLocation() - AveragePosition;
					//Clamp the magnitude of this vector so it will never push the sphere out past 100 units
					VecFromBoidsToSphereComp = VecFromBoidsToSphereComp.GetClampedToMaxSize(100.0f - (Member->SphereMesh->GetRelativeLocation() - Member->startingLocation).Size());

					//Move to new spot
					FinalPosition = FMath::VInterpTo(Member->SphereMesh->GetRelativeLocation(), Member->SphereMesh->GetRelativeLocation() + VecFromBoidsToSphereComp, DeltaTime, 1.0f);
					Member->SphereMesh->SetRelativeLocation(FinalPosition);

					Member->delay = 0.0f;

				}
				else
				{
					//return home after delay
					Member->delay = Member->delay + DeltaTime;
					if (Member->delay > ReturnHomeDelay)
					{
						// move back to the default position
						FinalPosition = FMath::VInterpTo(Member->SphereMesh->GetRelativeLocation(), Member->startingLocation, DeltaTime, 2.0f);
						Member->SphereMesh->SetRelativeLocation(FinalPosition);
					}
				}

				break;
			}
			default:
				break;
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SphereGridMember was Null! Fail to Tick Movement"))
		}
	}
}

