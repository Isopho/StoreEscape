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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	AcquirePhysicsHandleComponent();	
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if physics handle is attached 
	if (PhysicsHandle->GrabbedComponent) 
	{
		// move what player is holding
		auto ReachLine = GetReachLine();
		PhysicsHandle->SetTargetLocation(ReachLine.End);
	}
}


void UGrabber::Grab() {
	// The first object hit in reach.
	auto HitResult = GetFirstPhysicsBodyInReach();
	   	
	// The component that will be grabbed
	auto ComponentToGrab = HitResult.GetComponent();

	/// If we hit something then attach a physics handle
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, //no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
	
}


void UGrabber::ReleaseGrab() {
	// Release currently grabbed component
	PhysicsHandle->ReleaseComponent();
}


void UGrabber::AcquirePhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) 
	{
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent missing on %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) 
	{
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::ReleaseGrab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UInputComponent missing on %s"), *(GetOwner()->GetName()));
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	auto ReachLine = GetReachLine();

	/// Line-trace (AKA ray-cast) out to reach distance
	// Hit of the grab line trace.
	FHitResult Hit{};
	FCollisionQueryParams CQParams{NAME_None, false, GetOwner()};
	
	bool FoundHit = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		ReachLine.Start,
		ReachLine.End,
		FCollisionObjectQueryParams{ECollisionChannel::ECC_PhysicsBody},
		CQParams
	);

	/// see what we hit
	if (FoundHit) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(Hit.GetActor()->GetName()));
	}

	return Hit;
}

// Returns a struct with pair of Vectors that mark the start and the end of the reach line.
const UGrabber::FLine UGrabber::GetReachLine() const
{
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

	FLine ReachLine{ PlayerViewLocation, PlayerReachLocation };

	return ReachLine;
}


