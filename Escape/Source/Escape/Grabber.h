// Copyright Jan-Uriel Lorbeer 2019.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Public/DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Engine/World.h"
#include "Containers/Array.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Stuct to hold two FVectors (points in space) that make up a line.
	struct FLine
	{
		FVector Start;
		FVector End;

		//Constructor
		FLine(FVector LineStart, FVector LineEnd) : Start{ LineStart }, End{ LineEnd }
		{
		}
	};


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
		//How far the player can reach.
		float Reach = 180.0f;

	UPROPERTY()
		UPhysicsHandleComponent* PhysicsHandle{};

	UPROPERTY()
		UInputComponent* InputComponent{};

	// Ray-cast and grab whats in reach.
	void Grab();

	// Drop what is currently grabbed.
	void ReleaseGrab();

	// Find (assumed) attached UPhysicsHandleComponent
	void AcquirePhysicsHandleComponent();

	// Setup (assumed) attached InputComponent
	void SetupInputComponent();

	// Return hit for fist physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach() const;

	// Returns a struct with pair of Vectors that mark the start and the end of the reach line.
	const FLine GetReachLine() const;	
};