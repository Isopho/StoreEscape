// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivationReceiver.h"
#include "Engine/World.h"
#include "LaserBeam.h"
#include "LaserBeamReceiver.h"
#include "TableGameLaserController.generated.h"

/**
 * Player activatable (UActivationReveiver) ActorComponent that
 * that controls the activation and rotation behaviour of a LaserNode. 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPE_API UTableGameLaserController : public UActivationReceiver
{
	GENERATED_BODY()
	
public:

	UTableGameLaserController();

	// Inherited from UActivationReveiver. Starts the rotation of the LaserNode. 
	virtual void DoActivationAction() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// To determin if the Laser was activa for at least a certain time.
	UPROPERTY(BlueprintReadOnly)
		float CurrentActivationDuration{ 0.0f };

	// How long the LaserNode has to be continuously hit by a Laser to be activated.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ActivationDelay{ 0.5f };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// If the Node is moving/rotating.
	bool bMoving{ false };
	
	/// For the rotation Interpolation.
	FRotator StartRotation;
	FRotator TargetRotation;
	
	// Current duration of the ongoing move/rotation.
	float CurrentMoveTime{ 0.0f };

	// Time it takes the LaserNode to rotate upon activation.
	UPROPERTY(EditAnywhere)
		float MoveTime{ 0.5f };

	// How far the LaserNode should rotate upon activation in degrees.
	UPROPERTY(EditAnywhere)
		float TurnDegreesPerActivation{ 45.0f };

	// When true, the LaserNode does not rely on activation and is always on.
	UPROPERTY(EditAnywhere)
		bool bIsLaserBeamAlwaysOn{ false };

	// Laserhit counters to assert if continuously hit by a Laser.
	int32 LaserHitActivationCount{ 4 };
	int32 HitByLaser{ 0 };

	//UPROPERTY(EditAnywhere)
	//	float LaserActivationDelay{ 0.5f };
	
	/// Pointer handles by Unreal.
	UPROPERTY()
		ULaserBeam * LaserBeam{};	

	// Bound to LaserBeam received Event.
	UFUNCTION()
		void OnNotificationOfLaserBeamReceived(AActor* LaserBeamOriginActor);
};
