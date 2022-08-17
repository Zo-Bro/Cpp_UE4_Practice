// Fill out your copyright notice in the Description page of Project Settings.

#include "SlotMachine.h"
#include "Kismet/GameplayStatics.h"
#include "Practice_PuzzleBlock.h"
#include "SlotRoller.h"
#include "Components/TextRenderComponent.h"


// Sets default values
ASlotMachine::ASlotMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Slot_A_Position = FVector(0.0f, 0.0f, 0.0f);
	Slot_B_Position = FVector(0.0f, 200.0f, 0.0f);
	Slot_C_Position = FVector(0.0f, 400.0f, 0.0f);

	ColumnAText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Column A"));	
	ColumnBText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Column B"));
	ColumnCText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Column C"));
	
	SetRootComponent(ColumnAText);
	ColumnBText->AttachTo(RootComponent);
	ColumnCText->AttachTo(RootComponent);

	ColumnAText->SetRelativeLocation(Slot_A_Position + GetActorLocation());
	ColumnBText->SetRelativeLocation(Slot_B_Position);
	ColumnCText->SetRelativeLocation(Slot_C_Position);

	SlotDisplays.Add(ColumnAText);
	SlotDisplays.Add(ColumnBText);
	SlotDisplays.Add(ColumnCText);

	SlotA = CreateDefaultSubobject<USlotRoller>(TEXT("Slot A"));
	SlotB = CreateDefaultSubobject<USlotRoller>(TEXT("Slot B"));
	SlotC = CreateDefaultSubobject<USlotRoller>(TEXT("Slot C"));
	
	SpinTimer = 0.0f;
	IntervalTracker = 0.0f;
	
	ButtonXOffset = 50.0f;
	ButtonYOffset = 50.0f;


}
#if WITH_EDITOR
void ASlotMachine::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Re-calculate anything after making edits to a placed instance here

	//Set the root component in world space
	ColumnAText->SetRelativeLocation(Slot_A_Position + GetActorLocation());

	//Set the sub components relative to the root component
	ColumnBText->SetRelativeLocation(Slot_B_Position);
	ColumnCText->SetRelativeLocation(Slot_C_Position);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
// Called when the game starts or when spawned
void ASlotMachine::BeginPlay()
{
	Super::BeginPlay();

	// Make position vector, offset from Grid location
	const FVector BlockLocation = FVector(ButtonXOffset, ButtonYOffset, 0.f) + GetActorLocation();

	APractice_PuzzleBlock* SpinButton;
	SpinButton = GetWorld()->SpawnActor<APractice_PuzzleBlock>(BlockLocation, FRotator(0, 0, 0));
	// Set Up Spin Button Delegate
	if (SpinButton)
	{
		SpinButton->OnPuzzleBlockClickedDel.BindUObject(this, &ASlotMachine::Spin);
		SpinButton->SetupDelegates(this);
	}
	else
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APractice_PuzzleBlock::StaticClass(), FoundActors);
		if (APractice_PuzzleBlock* FoundBlock = Cast<APractice_PuzzleBlock>(FoundActors[0]))
		{
			FoundBlock->OnPuzzleBlockClickedDel.BindUObject(this, &ASlotMachine::Spin);
			FoundBlock->SetupDelegates(this);
			SpinButton = FoundBlock;
			// I want to call "Spin" everytime the Practice_PuzzleBlock calls its "OnPuzzleBlockClickedDel" delegate.
		}
		else
		{
			UE_LOG(LogCore, Warning, TEXT("Unable to bind Spin to a button. Slot Machine is useless."))
		}

	}

	//Set the default slots state
	SlotA->NextItem(ColumnAText);
	SlotB->NextItem(ColumnBText);
	SlotC->NextItem(ColumnCText);
	
}

// Called every frame
void ASlotMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Handle Spinning
	if (SpinTimer > 0.0f)
	{
		if (SlotA->Spin(DeltaTime))
		{
			SlotA->NextItem(ColumnAText);
		}
		if (SlotB->Spin(DeltaTime))
		{
			SlotB->NextItem(ColumnBText);
		}
		if (SlotC->Spin(DeltaTime))
		{
			SlotC->NextItem(ColumnCText);
		}
		SpinTimer = SpinTimer - DeltaTime;
		if (SpinTimer <= 0.0f)
		{
			OnDoneSpinningDel.ExecuteIfBound();
			SpinTimer = 0.0f;
		}
	}

}

void ASlotMachine::Spin()
{
	//Prep Spin Variables for OnTick
	SpinTimer = SpinLength;
	SpinTimer = SpinTimer + FMath::RandRange(0.0f, SpinVariance);
	SlotA->RandomizeInterval(MinSpinUpdateInterval, MaxSpinUpdateInterval);
	SlotB->RandomizeInterval(MinSpinUpdateInterval, MaxSpinUpdateInterval);
	SlotC->RandomizeInterval(MinSpinUpdateInterval, MaxSpinUpdateInterval);

}