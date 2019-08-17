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

/// Class for blueprint to activate the orbs flare, with the duration in seconds.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSimonOrbFlare, float, Duration, float, LightIntensity, float, VolumeMultiplyer);

/// Class for blueprint to activate the orbs flare, with the duration in seconds.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimonOrbGlow, float, Duration);

/// Class for blueprint to activate the orbs flare, with the duration in seconds.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSimonOrbBasicEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API USimonOrbController : public USwitch
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimonOrbController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FlareSimonOrb(float VolumeMultiplyer = 1.0f) const;

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
	float FlareTime = 1.0f;

	float FlareLightIntensity = 100.0f;

	UPROPERTY(EditAnywhere)
		float GlowLoopDuration = 10.0f;

	bool bPlayerActivatable = false;		

	bool bActivationBlocked = false;

	bool bIsGlowing = false;


};
