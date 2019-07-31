// Copyright Jan-Uriel Lorbeer 2019.


#include "SimonOrbController.h"

// Sets default values for this component's properties
USimonOrbController::USimonOrbController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USimonOrbController::ActivateActivatable()
{
	FlareSimonOrb();
}

// Called when the game starts
void USimonOrbController::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void USimonOrbController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetWorld()->GetTimeSeconds() - realtimeSeconds > 5.0f)
	{
		realtimeSeconds = GetWorld()->GetTimeSeconds();
	}


	// ...
}

void USimonOrbController::FlareSimonOrb() const
{
	FlareOrbRequest.Broadcast(1.0f);
}

