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

	LaserparticleSystemComponent = GetOwner()->FindComponentByClass<UParticleSystemComponent>();

	if (!LaserparticleSystemComponent->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString::Printf(TEXT("ULaserBeam:  The UParticleSystemComponent is missing on %s!"), *GetOwner()->GetName()));
	}

	if (LaserparticleSystemComponent->IsValidLowLevel())
	{
		LaserparticleSystemComponent->SetFloatParameter("BeamDistance", 0.0f);
		LaserparticleSystemComponent->SetActive(false);
	}
}

void ULaserBeam::UpdateLaserBeam()
{

	// The first object hit in the beam.
	if (bIsLaserBeamActivated)
	{
		auto HitResult = GetFirstActorInLaserBeamReach();

		/// If we hit something then attach a physics handle
		if (HitResult.GetActor())
		{
			// Update length of visable ParticleBeam
			if (LaserparticleSystemComponent->IsValidLowLevel())
			{
				LaserparticleSystemComponent->SetActive(true);
				LaserparticleSystemComponent->SetFloatParameter("BeamDistance", HitResult.Distance);
			}

			// If Beam hit a LaserBeam activated Actor, activate the LaserBeamReceiver.
			ULaserBeamReceiver* LaserBeamReceiver = HitResult.GetActor()->FindComponentByClass<ULaserBeamReceiver>();
			if (LaserBeamReceiver)
			{
				LaserBeamReceiver->DoLaserBeamReceivedAction();
			}

		}
		else
		{
			// Nothing hit: Set length of visable ParticleBeam to maximum.
			if (LaserparticleSystemComponent->IsValidLowLevel())
			{
				LaserparticleSystemComponent->SetActive(true);
				LaserparticleSystemComponent->SetFloatParameter("BeamDistance", MaximumLaserReach);
			}
		}
	}
	else
	{
		// LaserBeam not activated: "Disable" visable ParticleBeam.
		if (LaserparticleSystemComponent->IsValidLowLevel())
		{
			LaserparticleSystemComponent->SetFloatParameter("BeamDistance", 0.0f);
			LaserparticleSystemComponent->SetActive(false);
		}
	}
}

const FHitResult ULaserBeam::GetFirstActorInLaserBeamReach() const
{
	auto LaserLine = GetLaserLine();

	/// Line-trace (AKA ray-cast) out to reach distance
	// Hit of the line trace.
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
	// Exit if ParticleSystem is not valid.
	if (!LaserparticleSystemComponent->IsValidLowLevel()) {
		return FLine{ FVector{},FVector{} };
	}
	
	// LineTrace from the location and roation of the actual visable ParticleBeam.
	FVector LaserLocation{ LaserparticleSystemComponent->K2_GetComponentLocation() };
	FRotator LaserRotation{ LaserparticleSystemComponent->K2_GetComponentRotation() };
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

	UpdateLaserBeam();
	
}

void ULaserBeam::SetbIsLaserBeamActivated(bool bActivated)
{
	bIsLaserBeamActivated = bActivated;
}

bool ULaserBeam::IsLaserBeamActivated()
{
	return bIsLaserBeamActivated;
}

