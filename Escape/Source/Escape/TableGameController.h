// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Switch.h"
#include "Engine/World.h"
#include "TableGameController.generated.h"

/// Class for blueprint to activate the orbs flare, with the duration in seconds.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTableGameBasicEvent);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UTableGameController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTableGameController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FOnTableGameBasicEvent OnActivateGameRequest;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		AActor* ActivationSwitch {};

	UFUNCTION()
		void OnNotificationOfSwitchActivation(AActor* SwitchActor);
		
};
