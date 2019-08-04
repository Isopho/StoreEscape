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


void USimonOrbController::DoActivationAction()
{
	if (PlayerActivatable) {
		PlayerActivatable = false;
		FlareSimonOrb();

		OnSwitchActivation.Broadcast(GetOwner());

		FTimerHandle TimerHandle{};
		FTimerDelegate TimerDel{};
		TimerDel.BindUFunction(this, FName("SetPlayerActivatable"), true);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, FlareTime, false);
	}
}

void USimonOrbController::SetFlareTime(float FlareTime)
{
	this->FlareTime = FlareTime;
}

float USimonOrbController::GetFlareTime() const
{
	return FlareTime;
}

void USimonOrbController::SetPlayerActivatable(bool PlayerActivatable)
{
	this->PlayerActivatable = PlayerActivatable;
}

bool USimonOrbController::IsPlayerActivatable() const
{
	return PlayerActivatable;
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

	// ...
}

void USimonOrbController::FlareSimonOrb() const
{
	FlareOrbRequest.Broadcast(FlareTime);

}

