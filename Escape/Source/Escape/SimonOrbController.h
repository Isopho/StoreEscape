// Copyright Jan-Uriel Lorbeer 2019.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "GenericPlatform/GenericPlatformMath.h"

#include "ActivationReceiver.h"
#include "Switch.h"

#include "SimonOrbController.generated.h"

/// Classes for blueprint to activate/stop the orbs flare/glow, with parameters for duration in seconds, light flare intensity and sound volume.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSimonOrbFlare, float, Duration, float, LightIntensity, float, VolumeMultiplyer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimonOrbGlow, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSimonOrbBasicEvent);

/**
* Activatable (USwitch) ActorComponent to handle input (player activation) 
* and output (sound and light) for a simonorb.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API USimonOrbController : public USwitch
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimonOrbController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Flares the Simon orb
	void FlareSimonOrb(float VolumeMultiplyer = 1.0f) const;

	// Flares the Simon orb with custom duration and intensity.
	void FlareSimonOrb(float Duration, float LightIntensity, float VolumeMultiplyer = 1.0f);

	void SetSimonOrbGlow(bool Glowing);

	UPROPERTY(BlueprintAssignable)
		FOnSimonOrbFlare FlareOrbRequest;

	UPROPERTY(BlueprintAssignable)
		FOnSimonOrbGlow StartOrbGlowRequest;

	UPROPERTY(BlueprintAssignable)
		FOnSimonOrbBasicEvent StopOrbGlowRequest;

	void SetFlareTime(float FlareTime);

	float GetFlareTime() const;

	void SetFlareLightIntensity(float FlareLightIntensity);

	float GetFlareLightIntensity() const;
	
	UFUNCTION()
		void SetPlayerActivatable(bool PlayerActivatable);

	UFUNCTION()
		void SetActivationBlocked(bool ActivationBlocked);

	bool IsPlayerActivatable() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void DoActivationAction() override;

private:	
	// Duration of a flare animation.
	float FlareTime = 1.0f;

	// Base Intensity of the Simonorbs light.
	float FlareLightIntensity = 100.0f;

	// Duration of looping the glow animation in seconds.
	UPROPERTY(EditAnywhere)
		float GlowLoopDuration = 10.0f;

	// If the simonorb is activatable by the Player.
	bool bPlayerActivatable = false;		

	// If any kind of the activation is blocked.
	bool bActivationBlocked = false;

	// If the orb should glow.
	bool bIsGlowing = false;


};
