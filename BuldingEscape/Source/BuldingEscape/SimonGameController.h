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
enum SimonRoundStatus
{
	RoundLost			UMETA(DisplayName = "RoundLost"),
	ContinueRound		UMETA(DisplayName = "ContinueRound"),
	RoundWon			UMETA(DisplayName = "RoundWon"),
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

	bool IsGameWon() const;

	UFUNCTION()
	void OnNotificationOfSimonOrbActivation(AActor* ActivatedSimonOrb);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere)
		TArray<AActor*> SimonOrbs {};

	UPROPERTY(EditAnywhere)
		uint32 GameRoundsToWin {5};

	UPROPERTY(EditAnywhere)
		uint32 BaseGameSpeed = 1;
	
	uint32 CurrentGameRound{};

	bool GameWon = false;	

	void ResetGame();

	TArray<int32> GenerateRandomOrbSequence(int32 SequenceLength);

	bool StartGameRound();

	void PlayOrbSequence(TArray<int32> OrbSequence, float OrbFlareTime);

	UFUNCTION()
	void FlareOrb(int32 OrbNumber);

	TArray<int32> CurrentOrbSequenceTarget{};
	
	TArray<int32> CurrentOrbSequenceInput{};

	bool IsInputSequenceOkay();

	SimonRoundStatus CheckRoundStatus();

	UFUNCTION()
	void StarNewGame();

	UFUNCTION()
	void InitNextRound();

	UFUNCTION()
	void StartNewRound();

	void GameLost();

	bool AwaitingInput = false;

	float CurrentOrbFlareDuration{2.0f};
};
