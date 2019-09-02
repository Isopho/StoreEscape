// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Initiates a move of the actor to the TargetLocation.
	void Move();

	FVector GetStartLocation();

	FVector GetTargetLocation();

	// Target to move to. Used to set the movements target location and rotation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* MoveTarget;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

private:

	// Locations for the movement interpolation.
	FVector StartLocation;
	FVector TargetLocation;

	// Rotations for the movement interpolation.
	FRotator StartRotation;
	FRotator TargetRotation;

	bool bMoving{ false };
	
	// Current duration of the current move.
	float CurrentMoveTime{ 0.0f };
	
	// Duration of the move animation in seconds.
	UPROPERTY(EditAnywhere)
		float MoveTime{ 2.0f };

	void SwapStartAndTarget();

};
