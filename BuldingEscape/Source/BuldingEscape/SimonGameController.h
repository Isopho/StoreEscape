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
	NoState    					UMETA(DisplayName = "NoState"),
	Waiting						UMETA(DisplayName = "Waiting"),
	PreparingRound				UMETA(DisplayName = "PreparingRound"),
	DisplayingTargetSequence	UMETA(DisplayName = "ContinueRDisplayingTargetSequenceound"),
	AwaitingPlayerInput			UMETA(DisplayName = "AwaitingPlayerInput"),
	GameLost    				UMETA(DisplayName = "GameLost"),
	GameWon						UMETA(DisplayName = "GameWon"),
};


class  USimonGameState 
{
public:
	USimonGameState(const USimonGameController* SimonGameController);

	virtual ~USimonGameState();

	virtual void OnStateEnter();

	virtual void OnStateExit();

	virtual void OnPlayerInput(int32 OrbNumber);

	virtual ESimonGameState GetESimonGameState() const;

protected:

	TWeakObjectPtr<USimonGameController> SimonGameController {};

	ESimonGameState SimonGameState;

};

class  UPreparingRound : public USimonGameState 
{
public:
	UPreparingRound(const USimonGameController* SimonGameController) : USimonGameState(SimonGameController) { SimonGameState = ESimonGameState::PreparingRound; };

	virtual ESimonGameState GetESimonGameState() const override;

	virtual void OnStateEnter() override;

protected:	

	int32 GetRandomOrbNumber();

	TArray<int32> GenerateRandomOrbSequence(int32 SequenceLength);
	
	void InitNextRound();
};

class  UDisplayingTargetSequence : public USimonGameState 
{	
public:
	UDisplayingTargetSequence(const USimonGameController* SimonGameController) : USimonGameState(SimonGameController) { SimonGameState = ESimonGameState::DisplayingTargetSequence; };
	
	virtual ESimonGameState GetESimonGameState() const override;

	virtual void OnStateEnter() override;

protected:
	
	void PlayOrbSequence(TArray<int32> OrbSequence);

};

class  UAwaitingPlayerInput : public USimonGameState 
{	
public:
	UAwaitingPlayerInput(const USimonGameController* SimonGameController) : USimonGameState(SimonGameController) { SimonGameState = ESimonGameState::AwaitingPlayerInput; };

	virtual ~UAwaitingPlayerInput();

	virtual void OnStateEnter() override;

	virtual void OnStateExit() override;

	virtual void OnPlayerInput(int32 OrbNumber) override;
protected:

	TArray<int32> CurrentOrbSequenceInput{};

	ESimonRoundStatus CheckRoundStatus();

	bool IsInputSequenceOkay();
};

class  UGameWon : public USimonGameState 
{	
public:
	UGameWon(const USimonGameController* SimonGameController) : USimonGameState(SimonGameController) { SimonGameState = ESimonGameState::GameWon; };

protected:

};

class UGameLost : public USimonGameState 
{
public:
	UGameLost(const USimonGameController* SimonGameController) : USimonGameState(SimonGameController) { SimonGameState = ESimonGameState::GameLost; };

	virtual void OnStateEnter() override;
protected:


};

class UWaiting : public USimonGameState
{
public:
	UWaiting(const USimonGameController* SimonGameController) : USimonGameState(SimonGameController) { SimonGameState = ESimonGameState::Waiting; };

	virtual void OnStateEnter() override;

	virtual void OnStateExit() override;
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
		void SetSimonGameStateAfterDelay(ESimonGameState NewState, float Delay);

	UFUNCTION()
		void ResetGame();

	UFUNCTION()
		int32 GetNumberOfSimonOrbs() const;

	UFUNCTION()
		uint32 GetCurrentGameRound() const;

	UFUNCTION()
		uint32 GetGameRoundsToWin() const;

	UFUNCTION()
		void IncrementCurrentGameRound();
	
	UFUNCTION()
		TArray<int32> GetCurrentOrbSequenceTarget() const;

	UFUNCTION()
		void SetCurrentOrbSequenceTarget(TArray<int32> NewTarget);

	UFUNCTION()
		float GetBaseGameSpeed() const;

	UFUNCTION()
		float GetCurrentOrbFlareDuration() const;

	UFUNCTION()
		void SetCurrentOrbFlareDuration(float NewFlareDuration);

	UFUNCTION()
		void FlareOrb(int32 OrbNumber);

	UFUNCTION()
		void SetAllOrbsPlayerActivatable(bool Activatable);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	

	USimonGameState* CurrentSimonGameState;

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

	








	UFUNCTION()
	void StarNewGame();



	void GameIsLost();

	bool AwaitingInput = false;

};
