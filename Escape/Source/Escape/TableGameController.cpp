// Fill out your copyright notice in the Description page of Project Settings.


#include "TableGameController.h"

// Sets default values for this component's properties
UTableGameController::UTableGameController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTableGameController::BeginPlay()
{
	Super::BeginPlay();

	if (ActivationSwitch) {
		USwitch* SwitchComponent = ActivationSwitch->FindComponentByClass<USwitch>();

		if (SwitchComponent)
		{
			SwitchComponent->OnSwitchActivation.AddDynamic(this, &UTableGameController::OnNotificationOfSwitchActivation);
		}
	}
}

void UTableGameController::OnNotificationOfSwitchActivation(AActor* SwitchActor)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("OnNotificationOfSwitchActivation activated.")));
	OnActivateGameRequest.Broadcast();
}


// Called every frame
void UTableGameController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

