// Copyright Jan-Uriel Lorbeer 2019.


#include "Grabber.h"

// Makro to mark parameters as return values (only for code readability)
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewLocation{};
	FRotator PlayerViewRotation{};
	FVector PlayerViewDirection{};

	// Get player view point this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewLocation, 
		OUT PlayerViewRotation
	);

	PlayerViewDirection = PlayerViewRotation.Vector();

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s --- Rotation: %s"), *PlayerViewLocation.ToString(), *PlayerViewRotation.ToString());

	// ray-cast out to reach distance
	DrawDebugLine(
		GetWorld(),
		PlayerViewLocation,
		PlayerViewLocation + (PlayerViewDirection * Reach),
		FColor::Red,
		false,
		0.0f,
		0.0f,
		20.0f
	);

	// see what we hit
}

