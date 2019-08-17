// Copyright Jan-Uriel Lorbeer 2019.


#include "Switch.h"

// Sets default values for this component's properties
USwitch::USwitch()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USwitch::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USwitch::DoActivationAction()
{
	OnSwitchActivation.Broadcast(GetOwner());
}


// Called every frame
void USwitch::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}