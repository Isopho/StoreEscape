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


/**
* SimonGame state machine UActorComponent. Defines the behaviour of the
* SimonGame and handles communication with any registered simon orb.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API USimonGameController : public UActorComponent
{
	GENERATED_BODY()
		
public:	
	// Sets default values for this component's properties
	USimonGameController();

	~USimonGameController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		bool IsGameWon() const;

protected:

	virtual void BeginPlay() override;

private:	

	UFUNCTION()
		void OnNotificationOfSimonOrbActivation(AActor* ActivatedSimonOrb);

	UFUNCTION()
		void SetSimonGameState(ESimonGameState NewState);

	FTimerHandle SetSimonGameStateAfterDelay(ESimonGameState NewState, float Delay);

	UFUNCTION()
		void ResetGame();

	UFUNCTION()
		int32 GetNumberOfSimonOrbs() const;

	UFUNCTION()
		uint32 GetCurrentGameRound() const;

	UFUNCTION()
		uint32 GetGameRoundsToWin() const;

	UFUNCTION()
		float GetRoundTimeLimit() const;

	UFUNCTION()
		void IncrementCurrentGameRound();

	UFUNCTION()
		TArray<int32> GetCurrentOrbSequenceTarget() const;

	UFUNCTION()
		void SetCurrentOrbSequenceTarget(TArray<int32> NewTarget);

	UFUNCTION()
		float GetBaseOrbFlareDuration() const;

	UFUNCTION()
		float GetFlareWaitMultiplyer() const;

	UFUNCTION()
		float GetGameLostFlareDuration() const;

	UFUNCTION()
		float GetGameSpeedIncreaseRate() const;

	UFUNCTION()
		float GetDifficultyBumpLevels() const;

	UFUNCTION()
		float GetCurrentOrbFlareDuration() const;

	UFUNCTION()
		void SetCurrentOrbFlareDuration(float NewFlareDuration);

	UFUNCTION()
		float GetBaseOrbFlareLightIntensity() const;

	UFUNCTION()
		float GetGameWonOrbGlowDelay() const;

	UFUNCTION()
		float GetGameLostOrbFlareLightIntensity() const;

	UFUNCTION()
		float GetCurrentOrbFlareLightIntensity() const;

	UFUNCTION()
		void SetCurrentOrbFlareLightIntensity(float NewFlareLightIntensity);

	UFUNCTION()
		void SetGlowOnAllOrbs(bool Glowing);

	UFUNCTION()
		void SetGlowOnOrb(int32 OrbNumber, bool Glowing);

	UFUNCTION()
		void FlareOrb(int32 OrbNumber, float VolumeMultiplyer = 1.0f);

	UFUNCTION()
		void FlareAllOrbs(float Duration, float VolumeMultiplyer = 1.0f);

	UFUNCTION()
		void FlareOrbNTimes(int32 OrbNumber, int32 N, float VolumeMultiplyer = 1.0f);

	UFUNCTION()
		void FlareAllOrbsNTimes(float Duration, int32 N, float VolumeMultiplyer = 1.0f);

	UFUNCTION()
		void SetAllOrbsPlayerActivatable(bool Activatable);

	/// Pointers handled by Unreal
	// Pointers to all the SimonOrbs that are part of this SimonGame.
	UPROPERTY(EditAnywhere)
		TArray<AActor*> SimonOrbs {};

	UPROPERTY(EditAnywhere)
		uint32 GameRoundsToWin { 7 };

	UPROPERTY(EditAnywhere)
		float RoundTimeLimit{ 20.0f };

	UPROPERTY(EditAnywhere)
		float BaseOrbFlareDuration { 1.5f};

	UPROPERTY(EditAnywhere)
		float GameLostFlareDuration{ 0.25f };
	
	UPROPERTY(EditAnywhere)
		float BaseOrbFlareLightIntensity{ 150.0f };

	UPROPERTY(EditAnywhere)
		float GameLostOrbFlareLightIntensity{ 5.0f };

	UPROPERTY(EditAnywhere)
		float OrbGlowLightIntensity{ 15.0f };

	// Time between each orb glow activation during win animation in seconds .
	UPROPERTY(EditAnywhere)
		float GameWonOrbGlowDelay{ 0.3f };

	// Rate at which the game speed increases each round. GameSpeed always equals 1.0 in round 1.
	UPROPERTY(EditAnywhere)
		float GameSpeedIncreaseRate{ 0.334f };

	UPROPERTY(EditAnywhere)
		// Number of additional orb sequence steps increases every X levels. 0 for no increase.
		float DifficultyBumpLevels{ 5.0f };

	bool bGameIsWon = false;

	float CurrentOrbFlareDuration{ 2.0f };

	float CurrentOrbFlareLightIntensity{ BaseOrbFlareLightIntensity };

	uint32 CurrentGameRound{ 0 };

	TArray<int32> CurrentOrbSequenceTarget{};

	// Multiplayer to all flare wait times to avoid overlap.
	const float FlareWaitMultiplyer = 1.05f;

	// Parent Game State
	class  FSimonGameState
	{
	public:
		FSimonGameState(const USimonGameController* SimonGameController);

		virtual ~FSimonGameState();

		virtual void OnStateEnter();

		virtual void OnStateExit();

		virtual void OnPlayerInput(int32 OrbNumber);

		virtual ESimonGameState GetESimonGameState() const;

		virtual void NullSimonGameController();

	protected:

		// Weak SmartPointer to the StateMachine.
		TWeakObjectPtr<USimonGameController> SimonGameController{};

		ESimonGameState SimonGameState;

	};

	/**
	* Defines behaviour of the round preparation. Automatically transitions to the 
	* DisplayingTargetSequence state once preparations are completed.
	*/ 
	class  FPreparingRound : public FSimonGameState
	{
	public:
		FPreparingRound(const USimonGameController* SimonGameController) : FSimonGameState(SimonGameController) { SimonGameState = ESimonGameState::PreparingRound; };

		virtual ESimonGameState GetESimonGameState() const override;

		virtual void OnStateEnter() override;

	protected:

		int32 GetRandomOrbNumber();

		TArray<int32> GenerateRandomOrbSequence(int32 SequenceLength);

		void InitNextRound();
	};

	/**
	* Defines behaviour of the DisplayingTargetSequence game state. Displays the 
	* target orb sequence to the player. Automatically transitions to the AwaitingPlayerInput state onde done.
	*/
	class  FDisplayingTargetSequence : public FSimonGameState
	{
	public:
		FDisplayingTargetSequence(const USimonGameController* SimonGameController) : FSimonGameState(SimonGameController) { SimonGameState = ESimonGameState::DisplayingTargetSequence; };

		virtual ESimonGameState GetESimonGameState() const override;

		virtual void OnStateEnter() override;

	protected:

		void PlayOrbSequence(TArray<int32> OrbSequence);

	};

	/**
	* Defines behaviour of the AwaitingPlayerInput game state. 
	* Awaits Player input through the simon orbs. Transitions to
	* GameLost state if the wrong sequence was used by the player or time runs out
	* PreparingRound state if the sequence was correct and there are more rounds to play or
	* GameWon state if the sequence was correct and it was the last round.
	*/
	class  FAwaitingPlayerInput : public FSimonGameState
	{
	public:
		FAwaitingPlayerInput(const USimonGameController* SimonGameController) : FSimonGameState(SimonGameController) { SimonGameState = ESimonGameState::AwaitingPlayerInput; };

		virtual ~FAwaitingPlayerInput();

		virtual void OnStateEnter() override;

		virtual void OnStateExit() override;

		virtual void OnPlayerInput(int32 OrbNumber) override;
	protected:

		TArray<int32> CurrentOrbSequenceInput{};

		FTimerHandle RoundTimerHandle;

		ESimonRoundStatus CheckRoundStatus();

		bool IsInputSequenceOkay();
	};

	/**
	* Defines behaviour of the GameWon game state. Displays the Win animation.
	*/
	class  FGameWon : public FSimonGameState
	{
	public:
		FGameWon(const USimonGameController* SimonGameController) : FSimonGameState(SimonGameController) { SimonGameState = ESimonGameState::GameWon; };

		virtual void OnStateEnter() override;

	protected:

		void DisplayWinAnimation();

	};

	/**
	* Resets the SimonGame.
	* Automatically transitions to PreparingRound state once reset is complete.
	*/
	class FGameLost : public FSimonGameState
	{
	public:
		FGameLost(const USimonGameController* SimonGameController) : FSimonGameState(SimonGameController) { SimonGameState = ESimonGameState::GameLost; };

		virtual ~FGameLost();

		virtual void OnStateEnter() override;

		virtual void OnStateExit() override;

	protected:

	};

	/**
	* Does nothing until state is change from outside the state machine.
	*/
	class FWaiting : public FSimonGameState
	{
	public:
		FWaiting(const USimonGameController* SimonGameController) : FSimonGameState(SimonGameController) { SimonGameState = ESimonGameState::Waiting; };

		virtual void OnStateEnter() override;

		virtual void OnStateExit() override;
	protected:


	};

	FSimonGameState* CurrentSimonGameState;


};
