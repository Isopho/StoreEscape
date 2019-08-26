// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Widget.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Containers/Map.h"
#include "ObjectivesManager.generated.h"

/// Class for blueprint to activate the orbs flare, with the duration in seconds.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveEventTwo, FString, ObjectiveName, FString, ObjectiveText);

/// Class for blueprint to activate the orbs flare, with the duration in seconds.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveEventOne, FString, ObjectiveName);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UObjectivesManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectivesManager();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FOnObjectiveEventTwo OnAddObjectiveToHUD;

	UPROPERTY(BlueprintAssignable)
		FOnObjectiveEventTwo OnUpdateObjectiveInHUD;

	UPROPERTY(BlueprintAssignable)
		FOnObjectiveEventOne OnRemoveObjectiveFromHUD;

	UFUNCTION(BlueprintCallable)
		void AddObjective(FString ObjectiveName, FString ObjectiveText);

	UFUNCTION(BlueprintCallable)
		FString GetObjective(FString ObjectiveName);

	UFUNCTION(BlueprintCallable)
		void RemoveObjective(FString ObjectiveName);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TMap<FString, FString> ObjectivesMap{};

		
};
