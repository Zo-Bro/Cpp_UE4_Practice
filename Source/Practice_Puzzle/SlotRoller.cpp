// Fill out your copyright notice in the Description page of Project Settings.
#include "SlotRoller.h"

#include "Components/TextRenderComponent.h"

// Sets default values for this component's properties
USlotRoller::USlotRoller()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CurrentItem = 0;
	Interval = 0.1f;
	CurrentInterval = 0.0f;
	SlotOptions = TArray<FString>();
	SlotOptions.Add(FString("apple"));
	SlotOptions.Add(FString("banana"));
	SlotOptions.Add(FString("cheese"));


	// ...
}


// Called when the game starts
void USlotRoller::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USlotRoller::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USlotRoller::NextItem(UTextRenderComponent* TextToUpdate)
{
	if (SlotOptions.IsValidIndex(CurrentItem + 1))
	{
		++CurrentItem;
	}
	else
	{
		CurrentItem = 0;
	}
	TextToUpdate->SetText(FText::FromString(SlotOptions[CurrentItem]));
}

void USlotRoller::RandomizeInterval(float NewMinInterval, float NewMaxInterval)
{
	Interval = FMath::RandRange(NewMinInterval, NewMaxInterval);
}

bool USlotRoller::Spin(float DeltaSeconds)
{
	// Increment by delta seconds
	CurrentInterval = CurrentInterval + DeltaSeconds;
	float IntervalResult;
	// If we have achieved 1 full interval, it is time to change the current selected slot
	// so return true
	IntervalResult = CurrentInterval - Interval;
	if (IntervalResult >= 0)
	{
		CurrentInterval = IntervalResult;
		return true;
	}
	else
	{
		return false;
	}
}