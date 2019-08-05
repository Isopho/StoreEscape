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


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USimonGameState : public UObject {
	GENERATED_BODY()

public:
	USimonGameState();
	USimonGameState(USimonGameController* SimonGameController);
	~USimonGameState();

protected:

	UPROPERTY()
		TWeakObjectPtr<USimonGameController> SimonGameController {};

	UFUNCTION()
		virtual void OnStateEnter();

	UFUNCTION()
		virtual void OnStateExit();

	UFUNCTION()
		virtual void OnPlayerInput(int32 OrbNumber);

};

class UPreparingRound : public USimonGameState {
protected:
	UPreparingRound(USimonGameController* SimonGameController);

	UFUNCTION()
		virtual void OnStateEnter() override;

	UFUNCTION()
		TArray<int32> GenerateRandomOrbSequence(int32 SequenceLength);

	UFUNCTION()
		void InitNextRound();
};

class UDisplayingTargetSequence : public USimonGameState {
protected:
	UFUNCTION()
		virtual void OnStateEnter() override;

	UFUNCTION()
		void PlayOrbSequence(TArray<int32> OrbSequence);

};

class UAwaitingPlayerInput : public USimonGameState {
protected:
	UFUNCTION()
		virtual void OnStateEnter() override;

	UFUNCTION()
		virtual void OnPlayerInput(int32 OrbNumber) override;
};

class UGameWon : public USimonGameState {
protected:

};

class UGameLost : public USimonGameState {
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

	UPROPERTY()
	USimonGameState* CurrentSimonGameState;
};
