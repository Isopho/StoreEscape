// Copyright Jan-Uriel Lorbeer 2019.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

float TargetDegree{};
float TurnSpeed = -70.0f / 5;

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

	TargetDegree = -70.0f + GetOwner()->GetActorRotation().Yaw;	
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GetOwner()->GetActorRotation().Yaw > TargetDegree)
	{
		FRotator DoorRotator{ 0.0f,DeltaTime*TurnSpeed,0.0f };
		GetOwner()->AddActorLocalRotation(DoorRotator);
	}	
}

