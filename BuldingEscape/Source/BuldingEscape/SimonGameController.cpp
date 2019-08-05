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


uint32 USimonGameController::GetCurrentGameRound() const
{
	return CurrentGameRound;
}

void USimonGameController::IncrementCurrentGameRound()
{
	++CurrentGameRound;
}

TArray<int32> USimonGameController::GetCurrentOrbSequenceTarget()
{
	return CurrentOrbSequenceTarget;
}

void USimonGameController::SetCurrentOrbSequenceTarget(TArray<int32> NewTarget)
{
	CurrentOrbSequenceTarget = NewTarget;
}

float USimonGameController::GetBaseGameSpeed()
{
	return BaseGameSpeed;
}

float USimonGameController::GetCurrentOrbFlareDuration()
{
	return CurrentOrbFlareDuration;
}

void USimonGameController::SetCurrentOrbFlareDuration(float NewFlareDuration)
{
	CurrentOrbFlareDuration = NewFlareDuration;

	for (AActor* SimonOrb : SimonOrbs)
	{
		USimonOrbController* SimonOrbController = SimonOrb->FindComponentByClass<USimonOrbController>();
		if (SimonOrbController)
		{
			SimonOrbController->SetFlareTime(CurrentOrbFlareDuration);
		}
	}
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

	ResetGame();

	SetSimonGameState(ESimonGameState::PreparingRound);

}

void USimonGameController::ResetGame()
{
	//TODO remove awaiting input (should not be nessecary)
	AwaitingInput = false;

	CurrentGameRound = 4;
	CurrentOrbSequenceTarget.Empty();
}

int32 USimonGameController::GetNumberOfSimonOrbs() const
{
	return SimonOrbs.Num();
}




/// machine function
void USimonGameController::FlareOrb(int32 OrbNumber)
{	
	USimonOrbController* SimonOrbController = SimonOrbs[OrbNumber]->FindComponentByClass<USimonOrbController>();
	if (SimonOrbController)
	{
		SimonOrbController->FlareSimonOrb();
	}
}

/// input state
ESimonRoundStatus USimonGameController::CheckRoundStatus()
{/*
	if (!IsInputSequenceOkay())
	{
		return SimonRoundStatus::RoundLost;
	}
	else if (CurrentOrbSequenceInput.Num() == CurrentOrbSequenceTarget.Num())
	{
		return SimonRoundStatus::RoundWon;
	}*/
	return ESimonRoundStatus::ContinueRound;
}

//split to gamelost & prep round
void USimonGameController::StarNewGame()
{

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Starting new Simon Game.")));
	// reset game
	ResetGame();
}



/// input state
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


/// game over state
void USimonGameController::GameIsLost()
{

	FTimerHandle TimerHandle{};
	FTimerDelegate TimerDel{};
	TimerDel.BindUFunction(this, FName("StarNewGame"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, CurrentOrbFlareDuration * 2.0f, false);
	StarNewGame();
}


/// input state
bool USimonGameController::IsInputSequenceOkay()
{/*
	for (int32 i = 0; i < CurrentOrbSequenceInput.Num(); ++i) {
		if (CurrentOrbSequenceInput[i] != CurrentOrbSequenceTarget[i])
		{
			return false;
		}
	}*/
	return true;
}

/// useless
// Called every frame
void USimonGameController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/// machine function
bool USimonGameController::IsGameWon() const
{
	return GameIsWon;
}

/// machine & input state
void USimonGameController::OnNotificationOfSimonOrbActivation(AActor* ActivatedSimonOrb)
{
	if (!AwaitingInput || GameIsWon)
	{
		return;
	}

	for (int32 i = 0; i < SimonOrbs.Num(); ++i)
	{
		if (SimonOrbs[i] == ActivatedSimonOrb)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Notification: SimonOrb number %d was activated."), i));
			//CurrentOrbSequenceInput.Add(i);

			ESimonRoundStatus CurrentRoundStatus = CheckRoundStatus();

			// TODO check if done
			switch (CurrentRoundStatus)
			{
			case RoundLost:
				UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Game Lost in round %d."), CurrentGameRound));
				GameIsLost();
				break;
			case ContinueRound:
				break;
			case RoundWon:

				UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Round Won.")));
				if (CurrentGameRound == GameRoundsToWin)
				{

					UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Game won after %d rounds."), CurrentGameRound));
					GameIsWon = true;
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





void USimonGameController::SetSimonGameState(ESimonGameState NewState)
{
	switch (NewState)
	{
	case GameLost:
		CurrentSimonGameState = NewObject<UGameLost>(this);
		break;
	case GameWon:
		CurrentSimonGameState = NewObject<UGameWon>(this);
		break;
	case AwaitingPlayerInput:
		CurrentSimonGameState = NewObject<UAwaitingPlayerInput>(this);
		break;
	case DisplayingTargetSequence:
		CurrentSimonGameState = NewObject<UDisplayingTargetSequence>(this);
		break;
	case PreparingRound:
		CurrentSimonGameState = NewObject<UPreparingRound>(this);
		break;
	default:
		CurrentSimonGameState = NewObject<UGameLost>(this);
		break;
	}
}

/* FSimonGameState  Class*/
USimonGameState::USimonGameState()
{
	this->SimonGameController = nullptr;
	OnStateEnter();
}


USimonGameState::USimonGameState(USimonGameController* SimonGameController) 
{
	this->SimonGameController = SimonGameController;
	OnStateEnter();
}

USimonGameState::~USimonGameState()
{
	OnStateExit();
}

void USimonGameState::OnStateEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("USimonGameState OnStateEnter.")));
}

void USimonGameState::OnStateExit()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("USimonGameState OnStateExit.")));
}

void USimonGameState::OnPlayerInput(int32 OrbNumber)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("USimonGameState OnPlayerInput.")));
}



/* FSimonGameState UPreparingRound Class*/

UPreparingRound::UPreparingRound(USimonGameController* SimonGameController)
{
	this->SimonGameController = SimonGameController;
	OnStateEnter();
}

void UPreparingRound::OnStateEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("UPreparingRound OnStateEnter.")));

	InitNextRound();
	SimonGameController->SetSimonGameState(ESimonGameState::DisplayingTargetSequence);
}


TArray<int32> UPreparingRound::GenerateRandomOrbSequence(int32 SequenceLength)
{
	TArray<int32> OrbSequence{};

	for (int32 i = 0; i < SequenceLength; ++i)
	{
		OrbSequence.Push(FMath::RandRange(0, SimonGameController->GetNumberOfSimonOrbs() - 1));
	}

	return OrbSequence;
}

void UPreparingRound::InitNextRound()
{
	SimonGameController->IncrementCurrentGameRound();
	
	uint32 CurrentGameRound = SimonGameController->GetCurrentGameRound();


	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Initializing Round - %d"), CurrentGameRound));
	//generate sequence
	SimonGameController->SetCurrentOrbSequenceTarget ( GenerateRandomOrbSequence(CurrentGameRound) );


	// play sequence
	// set orb flare times
	SimonGameController->SetCurrentOrbFlareDuration(15.0f / (9 + (CurrentGameRound * SimonGameController->GetBaseGameSpeed())));
}



/* FSimonGameState UDisplayingTargetSequence Class*/

void UDisplayingTargetSequence::OnStateEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("UDisplayingTargetSequence OnStateEnter.")));

	PlayOrbSequence(SimonGameController->GetCurrentOrbSequenceTarget());

	// Switch to input state after flare
	FTimerHandle TimerHandle{};
	FTimerDelegate TimerDel{};
	TimerDel.BindUFunction(SimonGameController.Get(), FName("SetSimonGameState"), ESimonGameState::AwaitingPlayerInput);
	SimonGameController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, (SimonGameController->GetCurrentOrbSequenceTarget().Num()) * SimonGameController->GetCurrentOrbFlareDuration() * 1.05f, false);


	
}

void UDisplayingTargetSequence::PlayOrbSequence(TArray<int32> OrbSequence)
{
	SimonGameController->FlareOrb(OrbSequence[0]);

	for (int32 i = 1; i < OrbSequence.Num(); ++i)
	{
		FTimerHandle TimerHandle{};
		FTimerDelegate TimerDel{};
		TimerDel.BindUFunction(this, FName("FlareOrb"), OrbSequence[i]);
		SimonGameController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * SimonGameController->GetCurrentOrbFlareDuration() * 1.05f, false);
	}
}

void UAwaitingPlayerInput::OnStateEnter()
{
}

void UAwaitingPlayerInput::OnPlayerInput(int32 OrbNumber)
{
}
