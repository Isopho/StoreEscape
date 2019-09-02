// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "LaserBeamReceiver.h"
#include "LaserBeam.generated.h"

/**
* ActorComponent that manages the LaserBeam ParticleEmitter
* and handles activation of Actors with LaserBeamReveiver.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API ULaserBeam : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULaserBeam();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetbIsLaserBeamActivated(bool bActivated);

	bool IsLaserBeamActivated();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	struct FLine
	{
		FVector Start;
		FVector End;

		//Constructor
		FLine(FVector LineStart, FVector LineEnd) : Start{ LineStart }, End{ LineEnd }
		{
		}
	};

private:

	UPROPERTY(EditAnywhere)
		//How far the Laser can reach.
		float MaximumLaserReach = 10000.0f;
	   	 
	/// Pointer handled bei Unreal.
	UPROPERTY()
		UParticleSystemComponent * LaserparticleSystemComponent{};

	// (called every frame) Update the LaserBeamParticleEmitter and Actors hit by the laser based on a LineTrace.
	void UpdateLaserBeam();

	// Return hit for fist visible Actor in reach.
	const FHitResult GetFirstActorInLaserBeamReach() const;

	// Returns a struct with pair of Vectors that mark the start and the end of the reach line.
	const FLine GetLaserLine() const;
		
	bool bIsLaserBeamActivated{ true };

};
