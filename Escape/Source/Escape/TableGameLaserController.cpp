// Fill out your copyright notice in the Description page of Project Settings.


#include "TableGameLaserController.h"


UTableGameLaserController::UTableGameLaserController() {

}

void UTableGameLaserController::DoActivationAction()
{
	if (!bMoving) {
		StartRotation = GetOwner()->GetActorRotation();
		TargetRotation = StartRotation + FRotator{ 0,TurnDegreesPerActivation, 0 };
		bMoving = true;
	}

}



// Called every frame
void UTableGameLaserController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bMoving)
	{
		CurrentMoveTime += DeltaTime;
		AActor* Owner = GetOwner();
		float alpha = FMath::Min(1.0f, CurrentMoveTime / MoveTime);

		FRotator destinationRotation = FMath::InterpEaseInOut(StartRotation, TargetRotation, alpha, 2.0f);
		Owner->SetActorRotation(destinationRotation);


		if (CurrentMoveTime >= MoveTime)
		{
			CurrentMoveTime = 0.0f;
			bMoving = false;
		}
	}
}

void UTableGameLaserController::BeginPlay()
{
	Super::BeginPlay();
}

