// Copyright Jan-Uriel Lorbeer 2019.


#include "SimonGameController.h"

// Sets default values for this component's properties
USimonGameController::USimonGameController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimonGameController::BeginPlay()
{
	Super::BeginPlay();
	

	// TODO: check if all Actors in the SimonOrbs TArray have a SimonOrbController
	// remove the the ones that do not have one.


	// register gamecontroller remove the ones that are unsuccessful

	for (AActor* SimonOrb : SimonOrbs) 
	{
		USimonOrbController* SimonOrbController = SimonOrb->FindComponentByClass<USimonOrbController>();
		if (SimonOrbController)
		{
			// TODO check for failure and act accordingly
			SimonOrbController->OnSwitchActivation.AddDynamic(this, &USimonGameController::OnNotificationOfSimonOrbActivation);
		}
	}

	StarNewGame();

}

void USimonGameController::ResetGame()
{
	AwaitingInput = false;
	CurrentGameRound = 0;
	CurrentOrbSequenceTarget.Empty();
}

TArray<int32> USimonGameController::GenerateRandomOrbSequence(int32 SequenceLength)
{
	TArray<int32> OrbSequence{};

	for (int32 i = 0; i < SequenceLength; ++i)
	{
		OrbSequence.Push(FMath::RandRange(0, SimonOrbs.Num()-1));
	}

	return OrbSequence;
}

// Returns if the game round was won.
bool USimonGameController::StartGameRound()
{
	TArray<int32> OrbSequence{ GenerateRandomOrbSequence(CurrentGameRound) };

	// The time each orb glows in seconds.
	float FlareTimePerOrb = 20.0f / (9 + (CurrentGameRound * BaseGameSpeed));



	return false;
}

void USimonGameController::PlayOrbSequence(TArray<int32> OrbSequence, float OrbFlareTime)
{
	FlareOrb(OrbSequence[0]);

	for (int32 i = 1; i < OrbSequence.Num(); ++i) 
	{
		FTimerHandle TimerHandle{};
		FTimerDelegate TimerDel{};
		TimerDel.BindUFunction(this, FName("FlareOrb"), OrbSequence[i]);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * OrbFlareTime * 1.05f, false);
	}
}

void USimonGameController::FlareOrb(int32 OrbNumber)
{	
	USimonOrbController* SimonOrbController = SimonOrbs[OrbNumber]->FindComponentByClass<USimonOrbController>();
	if (SimonOrbController)
	{
		SimonOrbController->FlareSimonOrb();
	}
}

SimonRoundStatus USimonGameController::CheckRoundStatus()
{
	if (!IsInputSequenceOkay())
	{
		return SimonRoundStatus::RoundLost;
	}
	else if (CurrentOrbSequenceInput.Num() == CurrentOrbSequenceTarget.Num())
	{
		return SimonRoundStatus::RoundWon;
	}
	return SimonRoundStatus::ContinueRound;
}

void USimonGameController::StarNewGame()
{

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Starting new Simon Game.")));
	// reset game
	ResetGame();

	//start first round
	InitNextRound();
}

void USimonGameController::InitNextRound()
{
	AwaitingInput = false;
	CurrentOrbSequenceInput.Empty();
	++CurrentGameRound;


	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Initializing Round - %d"), CurrentGameRound));
	//generate sequence
	CurrentOrbSequenceTarget = GenerateRandomOrbSequence(CurrentGameRound);


	// play sequence
	// set orb flare times
	CurrentOrbFlareDuration = 15.0f / (9 + (CurrentGameRound * BaseGameSpeed));
	for (AActor* SimonOrb : SimonOrbs)
	{
		USimonOrbController* SimonOrbController = SimonOrb->FindComponentByClass<USimonOrbController>();
		if (SimonOrbController)
		{
			SimonOrbController->SetFlareTime(CurrentOrbFlareDuration);
		}
	}

	PlayOrbSequence(CurrentOrbSequenceTarget, CurrentOrbFlareDuration);

	// start round after flare
	FTimerHandle TimerHandle{};
	FTimerDelegate TimerDel{};
	TimerDel.BindUFunction(this, FName("StartNewRound"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, CurrentOrbSequenceTarget.Num() * CurrentOrbFlareDuration * 1.05f, false);
}

void USimonGameController::StartNewRound()
{

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Starting Round - %d"), CurrentGameRound));
	for (AActor* SimonOrb : SimonOrbs)
	{
		USimonOrbController* SimonOrbController = SimonOrb->FindComponentByClass<USimonOrbController>();
		if (SimonOrbController)
		{
			SimonOrbController->SetPlayerActivatable(true);
		}
	}
	AwaitingInput = true;
}

void USimonGameController::GameLost()
{

	FTimerHandle TimerHandle{};
	FTimerDelegate TimerDel{};
	TimerDel.BindUFunction(this, FName("StarNewGame"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, CurrentOrbFlareDuration * 2.0f, false);
	StarNewGame();
}


bool USimonGameController::IsInputSequenceOkay()
{
	for (int32 i = 0; i < CurrentOrbSequenceInput.Num(); ++i) {
		if (CurrentOrbSequenceInput[i] != CurrentOrbSequenceTarget[i])
		{
			return false;
		}
	}
	return true;
}


// Called every frame
void USimonGameController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);




}

bool USimonGameController::IsGameWon() const
{
	return GameWon;
}


void USimonGameController::OnNotificationOfSimonOrbActivation(AActor* ActivatedSimonOrb)
{
	if (!AwaitingInput || GameWon)
	{
		return;
	}

	for (int32 i = 0; i < SimonOrbs.Num(); ++i)
	{
		if (SimonOrbs[i] == ActivatedSimonOrb)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Notification: SimonOrb number %d was activated."), i));
			CurrentOrbSequenceInput.Add(i);

			SimonRoundStatus CurrentRoundStatus = CheckRoundStatus();

			// TODO check if done
			switch (CurrentRoundStatus)
			{
			case RoundLost:
				UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Game Lost in round %d."), CurrentGameRound));
				GameLost();
				break;
			case ContinueRound:
				break;
			case RoundWon:

				UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Round Won.")));
				if (CurrentGameRound == GameRoundsToWin)
				{

					UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Game won after %d rounds."), CurrentGameRound));
					GameWon = true;
				}
				else
				{
					FTimerHandle TimerHandle{};
					FTimerDelegate TimerDel{};
					TimerDel.BindUFunction(this, FName("InitNextRound"));
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, CurrentOrbFlareDuration * 2.0f, false);
				}
				break;
			default:
				break;
			}

		}

	}
}




