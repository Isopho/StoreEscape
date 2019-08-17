// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Switch.h"
#include "Engine/World.h"
#include "TableGameController.generated.h"

UENUM()
enum ETableGameState
{
	NoState    					UMETA(DisplayName = "NoState"),
	Waiting						UMETA(DisplayName = "Waiting"),
	GameRunning					UMETA(DisplayName = "GameRunning"),
	GameLost    				UMETA(DisplayName = "GameLost"),
	GameWon						UMETA(DisplayName = "GameWon"),
};


/// Class for blueprint to activate the orbs flare, with the duration in seconds.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTableGameBasicEvent);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UTableGameController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTableGameController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FOnTableGameBasicEvent OnActivateGameRequest;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		AActor* ActivationSwitch {};

	UFUNCTION()
		void OnNotificationOfSwitchActivation(AActor* SwitchActor);



	class  FTableGameState
	{
	public:
		FTableGameState(const UTableGameController* TableGameController);

		virtual ~FTableGameState();

		virtual void OnStateEnter();

		virtual void OnStateExit();

		virtual ETableGameState GetETableGameState() const;

	protected:

		TWeakObjectPtr<UTableGameController> TableGameController{};

		ETableGameState TableGameState;

	};


	class FWaiting : public FTableGameState
	{
	public:
		FWaiting(const UTableGameController* SimonGameController) : FTableGameState(SimonGameController) { TableGameState = ETableGameState::Waiting; };

	protected:

	};


	class FGameRunning : public FTableGameState
	{
	public:
		FGameRunning(const UTableGameController* SimonGameController) : FTableGameState(SimonGameController) { TableGameState = ETableGameState::GameRunning; };

	protected:

	};


	class FGameLost : public FTableGameState
	{
	public:
		FGameLost(const UTableGameController* SimonGameController) : FTableGameState(SimonGameController) { TableGameState = ETableGameState::GameLost; };

	protected:

	};


	class FGameWon : public FTableGameState
	{
	public:
		FGameWon(const UTableGameController* SimonGameController) : FTableGameState(SimonGameController) { TableGameState = ETableGameState::GameWon; };

	protected:

	};



		
};
