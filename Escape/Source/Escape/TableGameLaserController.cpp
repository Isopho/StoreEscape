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

	if (!bIsLaserBeamAlwaysOn)
	{
		if (HitByLaser == LaserHitActivationCount && CurrentActivationDuration < ActivationDelay)
		{
			CurrentActivationDuration += DeltaTime;
		}
		else if (HitByLaser > 0 && CurrentActivationDuration >= ActivationDelay )
		{
			LaserBeam->SetbIsLaserBeamActivated(true);
			--HitByLaser;
		}
		else
		{
			CurrentActivationDuration = 0.0f;
			LaserBeam->SetbIsLaserBeamActivated(false);
		}
	}
	else
	{
		LaserBeam->SetbIsLaserBeamActivated(true);
	}

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

	ULaserBeam* FoundLaserBeam = GetOwner()->FindComponentByClass<ULaserBeam>();
	if (FoundLaserBeam)
	{
		LaserBeam = FoundLaserBeam;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString::Printf(TEXT("UTableGameLaserController:  The ULaserBeam is missing on %s!"), *GetOwner()->GetName()));
	}

	ULaserBeamReceiver* LaserBeamReceiver = GetOwner()->FindComponentByClass<ULaserBeamReceiver>();
	if (LaserBeamReceiver)
	{
		LaserBeamReceiver->OnLaserBeamReceived.AddDynamic(this, &UTableGameLaserController::OnNotificationOfLaserBeamReceived);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString::Printf(TEXT("UTableGameLaserController:  The ULaserBeamReceiver is missing on %s!"), *GetOwner()->GetName()));
	}
	
}

void UTableGameLaserController::OnNotificationOfLaserBeamReceived(AActor* LaserBeamOriginActor)
{
	if (HitByLaser < LaserHitActivationCount)
	{
		++HitByLaser;
	}

}

