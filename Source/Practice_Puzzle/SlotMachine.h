// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlotMachine.generated.h"

DECLARE_DELEGATE(FDoneSpinning)

/*
ToDo:
- Add controls for text size
- create a widget for handling what appears in the slot machine
	- this widget should be controlled by a master password
	- would be dope if networking allowed the values to be updated on other clients (not necessary tho)
*/

UCLASS()
class PRACTICE_PUZZLE_API ASlotMachine : public AActor
{
	GENERATED_BODY()
	class UTextRenderComponent* ColumnAText;
	class UTextRenderComponent* ColumnBText;
	class UTextRenderComponent* ColumnCText;

	class USlotRoller* SlotA;
	class USlotRoller* SlotB;
	class USlotRoller* SlotC;

	
public:	
	// Sets default values for this actor's properties
	ASlotMachine();
	UPROPERTY(EditAnywhere, Category="Slot Options")
	TArray<FString> ColumnAOptions;

	UPROPERTY(EditAnywhere, Category="Slot Options")
	TArray<FString> ColumnBOptions;

	UPROPERTY(EditAnywhere, Category="Slot Options")
	TArray<FString> ColumnCOptions;

	TArray<UTextRenderComponent*> SlotDisplays;

	FDoneSpinning OnDoneSpinningDel;

	UPROPERTY(EditInstanceOnly, Category="Spin")
	float MaxSpinUpdateInterval;

	UPROPERTY(EditInstanceOnly, Category = "Spin")
	float MinSpinUpdateInterval;

	UPROPERTY(EditInstanceOnly, Category = "Spin")
	float SpinLength;

	UPROPERTY(EditInstanceOnly, Category = "Spin")
	float SpinVariance;

	UPROPERTY(EditInstanceOnly, Category = "Button")
	float ButtonXOffset;

	UPROPERTY(EditInstanceOnly, Category = "Button")
	float ButtonYOffset;

	UPROPERTY(EditAnywhere, Category = "Slot Display")
	FVector Slot_A_Position;
	
	UPROPERTY(EditAnywhere, Category = "Slot Display")
	FVector Slot_B_Position;

	UPROPERTY(EditAnywhere, Category = "Slot Display")
	FVector Slot_C_Position;


private:
	UFUNCTION()
	void Spin();
	//ToDo: Spin Timer that counts down each tick. At certain intervals, update the Rendered Text to one of the array values at random.
	// When the timer reaches zero, call the DoneSpinningDelegate.

	//USe End Spin Delegate to reset button. 	
	TArray<int> ActiveIndexPerSlot;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float SpinTimer;
	float IntervalTracker;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
