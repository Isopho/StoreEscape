// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "LaserBeamReceiver.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLaserBeamReceived, AActor*, LaserBeamReceivedActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API ULaserBeamReceiver : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULaserBeamReceiver();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FOnLaserBeamReceived OnLaserBeamReceived;

	virtual void DoLaserBeamReceivedAction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
