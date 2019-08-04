// Copyright Jan-Uriel Lorbeer 2019.


#include "Activator.h"

// Makro to mark parameters as return values (only for code readability)
#define OUT

// Sets default values for this component's properties
UActivator::UActivator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UActivator::BeginPlay()
{
	Super::BeginPlay();

	SetupInputComponent();
}


// Called every frame
void UActivator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}


void UActivator::ActivateAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to use something."));

	// The first object hit in reach.
	auto HitResult = GetFirstActorInReach();



	// The component that will be grabbed
	//auto ComponentToGrab = HitResult.GetComponent();

	/// If we hit something then attach a physics handle
	if (HitResult.GetActor())
	{
		UActivationReceiver* ActivationReceiver = HitResult.GetActor()->FindComponentByClass<UActivationReceiver>();
		UE_LOG(LogTemp, Warning, TEXT("Trying to use %s"), *(HitResult.GetActor()->GetName()));

		if (ActivationReceiver)
		{
			ActivationReceiver->DoActivationAction();
			UE_LOG(LogTemp, Warning, TEXT("Used %s"), *(HitResult.GetActor()->GetName()));
		}
	}

}


void UActivator::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Activate", EInputEvent::IE_Pressed, this, &UActivator::ActivateAction);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UInputComponent missing on %s"), *(GetOwner()->GetName()));
	}
}

const FHitResult UActivator::GetFirstActorInReach() const
{
	auto ReachLine = GetReachLine();

	/// Line-trace (AKA ray-cast) out to reach distance
	// Hit of the grab line trace.
	FHitResult Hit{};
	FCollisionQueryParams CQParams{ NAME_None, false, GetOwner() };

	bool FoundHit = GetWorld()->LineTraceSingleByChannel(
		OUT Hit,
		ReachLine.Start,
		ReachLine.End,
		ECollisionChannel::ECC_Visibility,
		CQParams
	);
	return Hit;
}

// Returns a struct with pair of Vectors that mark the start and the end of the reach line.
const UActivator::FLine UActivator::GetReachLine() const
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


