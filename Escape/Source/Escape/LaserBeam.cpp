// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBeam.h"

// Sets default values for this component's properties
ULaserBeam::ULaserBeam()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULaserBeam::BeginPlay()
{
	Super::BeginPlay();

	// ...
	LaserparticleSystemComponent = GetOwner()->FindComponentByClass<UParticleSystemComponent>();
}

void ULaserBeam::UpdateLaserBeam()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing Laser..."));

	// The first object hit in the beam.
	auto HitResult = GetFirstActorInLaserBeamReach();
	

	/// If we hit something then attach a physics handle
	if (HitResult.GetActor())
	{
		/*
		UActivationReceiver* ActivationReceiver = HitResult.GetActor()->FindComponentByClass<UActivationReceiver>();
		UE_LOG(LogTemp, Warning, TEXT("Trying to use %s"), *(HitResult.GetActor()->GetName()));

		if (ActivationReceiver)
		{
			ActivationReceiver->DoActivationAction();
			UE_LOG(LogTemp, Warning, TEXT("Used %s"), *(HitResult.GetActor()->GetName()));
		}*/
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Laser hit %s after a distance of %f!"), *HitResult.GetActor()->GetName(), HitResult.Distance));

		//FName ParamName = *FString::Printf(TEXT("BeamDistance"));

		if (LaserparticleSystemComponent->IsValidLowLevel()) {
			LaserparticleSystemComponent->SetFloatParameter("BeamDistance", HitResult.Distance);
		}
	}
}

const FHitResult ULaserBeam::GetFirstActorInLaserBeamReach() const
{
	auto LaserLine = GetLaserLine();

	/// Line-trace (AKA ray-cast) out to reach distance
	// Hit of the grab line trace.
	FHitResult Hit{};
	FCollisionQueryParams CQParams{ NAME_None, false, GetOwner() };

	bool FoundHit = GetWorld()->LineTraceSingleByChannel(
		OUT Hit,
		LaserLine.Start,
		LaserLine.End,
		ECollisionChannel::ECC_Visibility,
		CQParams
	);
	return Hit;
}

const ULaserBeam::FLine ULaserBeam::GetLaserLine() const
{
	// Location of the laser
	if (!LaserparticleSystemComponent->IsValidLowLevel()) {
		return FLine{ FVector{},FVector{} };
	}
	FVector LaserLocation{ LaserparticleSystemComponent->K2_GetComponentLocation() };
	FRotator LaserRotation{ GetOwner()->GetActorRotation() };
	FVector LaserDirection{};
	FVector LaserMaxReachLocation{};

	LaserDirection = LaserRotation.Vector();
	LaserMaxReachLocation = LaserLocation + (LaserDirection * MaximumLaserReach);

	FLine LaserLine{ LaserLocation, LaserMaxReachLocation };

	return LaserLine;
}



// Called every frame
void ULaserBeam::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	count++;
	if (count > 100)
	{
		count = 0;
		UpdateLaserBeam();
	}
}

