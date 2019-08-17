// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TableGameController.generated.h"

//UENUM()
//enum ETableGameState
//{
//	NoState    					UMETA(DisplayName = "NoState"),
//	Waiting						UMETA(DisplayName = "Waiting"),
//	GameRunning					UMETA(DisplayName = "GameRunning"),
//	GameLost    				UMETA(DisplayName = "GameLost"),
//	GameWon						UMETA(DisplayName = "GameWon"),
//};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UTableGameController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTableGameController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
