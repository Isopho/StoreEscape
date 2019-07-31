// Copyright Jan-Uriel Lorbeer 2019.


#include "Activatable.h"

// Sets default values for this component's properties
UActivatable::UActivatable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActivatable::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActivatable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActivatable::ActivateActivatable()
{
	UE_LOG(LogTemp, Warning, TEXT("Virtual Activatable Activated on %s"), *(GetOwner()->GetName()));
}

