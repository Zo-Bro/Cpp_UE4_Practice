// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SlotRoller.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICE_PUZZLE_API USlotRoller : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlotRoller();
	
	UFUNCTION()
	void NextItem(UTextRenderComponent* TextToUpdate);

	UFUNCTION()
	void RandomizeInterval(float NewMinInterval, float NewMaxInterval);

	UFUNCTION()
	bool Spin(float DeltaSeconds);

private:

	UPROPERTY()
	TArray<FString> SlotOptions;

	UPROPERTY()
	int CurrentItem;

	UPROPERTY()
	float Interval;

	float CurrentInterval;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
