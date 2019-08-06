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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimonOrbFlare, float, FlareDuration);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BULDINGESCAPE_API USimonOrbController : public USwitch
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimonOrbController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FlareSimonOrb() const;

	UPROPERTY(BlueprintAssignable)
		FOnSimonOrbFlare FlareOrbRequest;

	virtual void DoActivationAction() override;

	void SetFlareTime(float FlareTime);

	float GetFlareTime() const;
	
	UFUNCTION()
		void SetPlayerActivatable(bool PlayerActivatable);

	UFUNCTION()
		void SetActivationBlocked(bool ActivationBlocked);

	bool IsPlayerActivatable() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	float FlareTime = 1.0f;

	bool PlayerActivatable = false;		

	bool ActivationBlocked = false;
};
