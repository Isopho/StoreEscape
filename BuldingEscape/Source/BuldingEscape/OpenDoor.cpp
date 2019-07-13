// Copyright Jan-Uriel Lorbeer 2019.


#include "OpenDoor.h"

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
	ActivatingActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActivatingActor) && std::abs(GetOwner()->GetActorRotation().Yaw - (StartDegree + OpenAngle)) > 1.0f)
	{
		FRotator DoorRotator{ 0.0f,DeltaTime * OpenAngle / DoorOpenCloseDuration,0.0f };
		GetOwner()->AddActorLocalRotation(DoorRotator);
	}	
	else if (!PressurePlate->IsOverlappingActor(ActivatingActor) && std::abs(GetOwner()->GetActorRotation().Yaw - StartDegree) > 1.0f)
	{
		FRotator DoorRotator{ 0.0f,DeltaTime * (-OpenAngle) / DoorOpenCloseDuration,0.0f };
		GetOwner()->AddActorLocalRotation(DoorRotator);
	}
}

