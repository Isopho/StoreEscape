// Copyright Jan-Uriel Lorbeer 2019.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "SimonOrbController.h"
#include "SimonGameController.generated.h"

class USimonOrbController;

UENUM()
enum ESimonRoundStatus
{
	RoundLost			UMETA(DisplayName = "RoundLost"),
	ContinueRound		UMETA(DisplayName = "ContinueRound"),
	RoundWon			UMETA(DisplayName = "RoundWon"),
};

UENUM()
enum ESimonGameState
{
	GameLost    				UMETA(DisplayName = "GameLost"),
	GameWon						UMETA(DisplayName = "GameWon"),
	AwaitingPlayerInput			UMETA(DisplayName = "AwaitingPlayerInput"),
	DisplayingTargetSequence	UMETA(DisplayName = "ContinueRDisplayingTargetSequenceound"),
	PreparingRound				UMETA(DisplayName = "PreparingRound"),
};


class  USimonGameState 
{

public:
	USimonGameState();
	virtual ~USimonGameState();

		virtual void OnStateEnter(USimonGameController* SimonGameController);

		virtual void OnPlayerInput(int32 OrbNumber);

protected:

		TWeakObjectPtr<USimonGameController> SimonGameController {};


};

class  UPreparingRound : public USimonGameState 
{
public:
	UPreparingRound();

	virtual void OnStateEnter(USimonGameController* SimonGameController) override;

protected:
	
	
		TArray<int32> GenerateRandomOrbSequence(int32 SequenceLength);

	
		void InitNextRound();
};

class  UDisplayingTargetSequence : public USimonGameState 
{
	
public:

	virtual void OnStateEnter(USimonGameController* SimonGameController) override;

protected:

	
		void PlayOrbSequence(TArray<int32> OrbSequence);

};

class  UAwaitingPlayerInput : public USimonGameState 
{
	
public:

protected:

};

class  UGameWon : public USimonGameState 
{
	
public:

protected:

};

class UGameLost : public USimonGameState 
{
	protected:

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BULDINGESCAPE_API USimonGameController : public UActorComponent
{
	GENERATED_BODY()
		
public:	
	// Sets default values for this component's properties
	USimonGameController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		bool IsGameWon() const;

	UFUNCTION()
		void OnNotificationOfSimonOrbActivation(AActor* ActivatedSimonOrb);
	   
	UFUNCTION()
		void SetSimonGameState(ESimonGameState NewState);

	UFUNCTION()
		void ResetGame();

	UFUNCTION()
		int32 GetNumberOfSimonOrbs() const;

	UFUNCTION()
		uint32 GetCurrentGameRound() const;

	UFUNCTION()
		void IncrementCurrentGameRound();
	
	UFUNCTION()
		TArray<int32> GetCurrentOrbSequenceTarget();

	UFUNCTION()
		void SetCurrentOrbSequenceTarget(TArray<int32> NewTarget);

	UFUNCTION()
		float GetBaseGameSpeed();

	UFUNCTION()
		float GetCurrentOrbFlareDuration();

	UFUNCTION()
		void SetCurrentOrbFlareDuration(float NewFlareDuration);

	UFUNCTION()
		void FlareOrb(int32 OrbNumber);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere)
		TArray<AActor*> SimonOrbs {};

	UPROPERTY(EditAnywhere)
		uint32 GameRoundsToWin {5};

	UPROPERTY(EditAnywhere)
		float BaseGameSpeed {1.0f};
		
	uint32 CurrentGameRound{0};

	TArray<int32> CurrentOrbSequenceTarget{};

	bool GameIsWon = false;	

	float CurrentOrbFlareDuration{ 2.0f };









	

	bool IsInputSequenceOkay();

	ESimonRoundStatus CheckRoundStatus();

	UFUNCTION()
	void StarNewGame();


	UFUNCTION()
	void StartNewRound();

	void GameIsLost();

	bool AwaitingInput = false;

	USimonGameState* CurrentSimonGameState;
};
