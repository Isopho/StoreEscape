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



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API USimonGameController : public UActorComponent
{
	GENERATED_BODY()
		
public:	
	// Sets default values for this component's properties
	USimonGameController();

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


	UPROPERTY(EditAnywhere)
		TArray<AActor*> SimonOrbs {};

	UPROPERTY(EditAnywhere)
		uint32 GameRoundsToWin { 7 };

	UPROPERTY(EditAnywhere)
		float RoundTimeLimit{ 5.0f };

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

	UPROPERTY(EditAnywhere)
		float GameWonOrbGlowDelay{ 0.3f };

	UPROPERTY(EditAnywhere)
		float GameSpeedIncreaseRate{ 0.334f };

	UPROPERTY(EditAnywhere)
		// Number of additional orb steps increases every X levels
		float DifficultyBumpLevels{ 5.0f };

	bool bGameIsWon = false;

	float CurrentOrbFlareDuration{ 2.0f };

	float CurrentOrbFlareLightIntensity{ BaseOrbFlareLightIntensity };

	uint32 CurrentGameRound{ 0 };

	TArray<int32> CurrentOrbSequenceTarget{};

	const float FlareWaitMultiplyer = 1.05f;


	class  FSimonGameState
	{
	public:
		FSimonGameState(const USimonGameController* SimonGameController);

		virtual ~FSimonGameState();

		virtual void OnStateEnter();

		virtual void OnStateExit();

		virtual void OnPlayerInput(int32 OrbNumber);

		virtual ESimonGameState GetESimonGameState() const;

	protected:

		TWeakObjectPtr<USimonGameController> SimonGameController{};

		ESimonGameState SimonGameState;

	};

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

	class  FDisplayingTargetSequence : public FSimonGameState
	{
	public:
		FDisplayingTargetSequence(const USimonGameController* SimonGameController) : FSimonGameState(SimonGameController) { SimonGameState = ESimonGameState::DisplayingTargetSequence; };

		virtual ESimonGameState GetESimonGameState() const override;

		virtual void OnStateEnter() override;

	protected:

		void PlayOrbSequence(TArray<int32> OrbSequence);

	};

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

	class  FGameWon : public FSimonGameState
	{
	public:
		FGameWon(const USimonGameController* SimonGameController) : FSimonGameState(SimonGameController) { SimonGameState = ESimonGameState::GameWon; };

		virtual void OnStateEnter() override;

	protected:

		void DisplayWinAnimation();

	};

	class FGameLost : public FSimonGameState
	{
	public:
		FGameLost(const USimonGameController* SimonGameController) : FSimonGameState(SimonGameController) { SimonGameState = ESimonGameState::GameLost; };

		virtual ~FGameLost();

		virtual void OnStateEnter() override;

		virtual void OnStateExit() override;

	protected:

	};

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
