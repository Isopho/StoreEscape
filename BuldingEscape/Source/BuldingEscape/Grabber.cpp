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

	///Look for attached Physics Handle

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent missing on %s"), *(GetOwner()->GetName()));
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Location of the Player Pawn Kamera
	FVector PlayerViewLocation{};
	FRotator PlayerViewRotation{};
	FVector PlayerViewDirection{};
	FVector PlayerReachLocation{};
	/// Get player view point this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewLocation, 
		OUT PlayerViewRotation
	);

	PlayerViewDirection = PlayerViewRotation.Vector();
	PlayerReachLocation = PlayerViewLocation + (PlayerViewDirection * Reach);

	///UE_LOG(LogTemp, Warning, TEXT("Location: %s --- Rotation: %s"), *PlayerViewLocation.ToString(), *PlayerViewRotation.ToString());

	/// ray-cast out to reach distance
	DrawDebugLine(
		GetWorld(),
		PlayerViewLocation,
		PlayerReachLocation,
		FColor::Red,
		false,
		0.0f,
		0.0f,
		20.0f
	);

	/// Line-trace (AKA ray-cast) out to reach distance
	// Hit of the grab line trace.
	FHitResult Hit{};
	FCollisionQueryParams CQParams{FName(TEXT("")), false, GetOwner()};
	
	bool FoundHit = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewLocation,
		PlayerReachLocation,
		FCollisionObjectQueryParams{ECollisionChannel::ECC_PhysicsBody},
		CQParams
	);

	/// see what we hit
	if (FoundHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(Hit.GetActor()->GetName()));
	}

}

