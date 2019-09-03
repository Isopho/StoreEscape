// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	// Event: Places an objective in the hud and registers it under a name.
	UPROPERTY(BlueprintAssignable)
		FOnObjectiveEventTwo OnAddObjectiveToHUD;

	// Event: Updates the text of a objective in the hud.
	UPROPERTY(BlueprintAssignable)
		FOnObjectiveEventTwo OnUpdateObjectiveInHUD;

	// Event: Removes objective with the given name from the hud.
	UPROPERTY(BlueprintAssignable)
		FOnObjectiveEventOne OnRemoveObjectiveFromHUD;

	// Registers a new objective to be managed or updates an existing objective if the ObjectiveName already exists.
	UFUNCTION(BlueprintCallable)
		void AddObjective(FString ObjectiveName, FString ObjectiveText);

	// Returns the text of the requested objective.
	UFUNCTION(BlueprintCallable)
		FString GetObjective(FString ObjectiveName);

	// Removes the requested objective.
	UFUNCTION(BlueprintCallable)
		void RemoveObjective(FString ObjectiveName);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Map over all objecitves and their texts <ObjectiveName, ObjectiveText>
	TMap<FString, FString> ObjectivesMap{};

		
};
