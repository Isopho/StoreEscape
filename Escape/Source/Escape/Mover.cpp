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
	TargetLocation = MoveTarget->GetActorLocation();

	// ...
	
}

float UMover::CalSmootherStep(float Time)
{
	if (Time < 1.0f) {
		return Time * Time * Time * (Time * (Time * 6 - 15) + 10);
	}
	else
	{
		return 1.0f;
	}
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bMoving)
	{
		AActor* Owner = GetOwner();
		FVector destinationLocation = FMath::VInterpTo(Owner->GetActorLocation(), TargetLocation, DeltaTime, 0.5f);
		Owner->SetActorLocation(destinationLocation);
	}
}

void UMover::Move()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Mover called")));
	bMoving = true;
}

