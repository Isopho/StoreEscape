// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetOwner()->GetActorLocation();
	StartRotation = GetOwner()->GetActorRotation();

	if (MoveTarget) {
		TargetLocation = MoveTarget->GetActorLocation();
		TargetRotation = MoveTarget->GetActorRotation();
	}

}

void UMover::SwapStartAndTarget()
{
	FVector tmpL = StartLocation;
	StartLocation = TargetLocation;
	TargetLocation = tmpL;


	FRotator tmpR = StartRotation;
	StartRotation = TargetRotation;
	TargetRotation = tmpR;
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bMoving)
	{
		CurrentMoveTime += DeltaTime;
		AActor* Owner = GetOwner();
		float alpha = FMath::Min(1.0f, CurrentMoveTime / MoveTime);

		FVector destinationLocation = FMath::InterpEaseInOut(StartLocation, TargetLocation, alpha, 2.0f);
		Owner->SetActorLocation(destinationLocation);

		FRotator destinationRotation = FMath::InterpEaseInOut(StartRotation, TargetRotation, alpha, 2.0f);
		Owner->SetActorRotation(destinationRotation);


		if (CurrentMoveTime >= MoveTime)
		{
			CurrentMoveTime = 0.0f;
			bMoving = false;
			SwapStartAndTarget();
		}
	}
}

void UMover::Move()
{
	if (!bMoving)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Mover called")));
		if (MoveTarget)
		{
			bMoving = true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s"), *FString::Printf(TEXT("Mover called with no move target!")));
		}
	}	
}

FVector UMover::GetStartLocation()
{
	return StartLocation;
}

FVector UMover::GetTargetLocation()
{
	return TargetLocation;
}

