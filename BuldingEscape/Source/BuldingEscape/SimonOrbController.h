// Copyright Jan-Uriel Lorbeer 2019.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Engine/TriggerVolume.h"
#include "GenericPlatform/GenericPlatformMath.h"

#include "Activatable.h"

#include "SimonOrbController.generated.h"

/// Class for blueprint to activate the orbs flare, with the duration in seconds.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimonOrbFlare, float, FlareDuration);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BULDINGESCAPE_API USimonOrbController : public UActivatable
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

	virtual void ActivateActivatable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	float realtimeSeconds{};

		
};
