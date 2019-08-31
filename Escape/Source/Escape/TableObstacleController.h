// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"

#include "Switch.h"
#include "Mover.h"

#include "TableObstacleController.generated.h"

/**
 * 
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivationAction);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPE_API UTableObstacleController : public USwitch 
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTableObstacleController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FOnActivationAction OnActivationAction;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	virtual void DoActivationAction() override;

};
