// Copyright Jan-Uriel Lorbeer 2019.
#include "OpenDoor.h"

// Makro to mark parameters as return values (only for code readability)
#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	StartDegree = GetOwner()->GetActorRotation().Yaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("TriggerVolume missing on %s"), *(GetOwner()->GetName()));
	}
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActorsOnTriggerArea() > TriggerMass)
	{
		OpenDoor();
	} 
	else
	{
		CloseDoor();
	}
}

float UOpenDoor::TotalMassOfActorsOnTriggerArea()
{
	if (!PressurePlate) { return 0.0f; }

	float TotalMass = 0.0f;
	
	// Find all actors in/on trigger area
	TArray<AActor*> OverlappingActors{};

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// sum up their masses
	for (const auto * OverlappingActor : OverlappingActors) 
	{		
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::OpenDoor()
{
	OpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	CloseRequest.Broadcast();
}

