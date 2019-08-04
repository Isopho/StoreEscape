// Copyright Jan-Uriel Lorbeer 2019.


#include "ActivationReceiver.h"

// Sets default values for this component's properties
UActivationReceiver::UActivationReceiver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActivationReceiver::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UActivationReceiver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActivationReceiver::DoActivationAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Virtual Activatable Activated on %s"), *(GetOwner()->GetName()));
}
