// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Move();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* MoveTarget;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

private:

	FVector StartLocation;
	FVector TargetLocation;

	bool bMoving{ false };

	uint32 Times = 0;

	float CalSmootherStep(float Time);

	UPROPERTY(EditAnywhere)
		float MoveSpeedInterpolator{ 1.0f };

};
