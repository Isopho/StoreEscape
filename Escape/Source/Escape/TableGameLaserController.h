// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivationReceiver.h"
#include "Engine/World.h"
#include "TableGameLaserController.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPE_API UTableGameLaserController : public UActivationReceiver
{
	GENERATED_BODY()
	
public:

	UTableGameLaserController();

	virtual void DoActivationAction() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool bMoving{ false };
	
	FRotator StartRotation;
	FRotator TargetRotation;

	uint32 Times = 0;

	float CurrentMoveTime{ 0.0f };

	UPROPERTY(EditAnywhere)
		float MoveTime{ 0.5f };

	UPROPERTY(EditAnywhere)
		float TurnDegreesPerActivation{ 45.0f };
};
