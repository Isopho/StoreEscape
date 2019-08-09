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

uint32 USimonGameController::GetGameRoundsToWin() const
{
	return GameRoundsToWin;
}

void USimonGameController::IncrementCurrentGameRound()
{
	++CurrentGameRound;
}

TArray<int32> USimonGameController::GetCurrentOrbSequenceTarget() const
{
	return CurrentOrbSequenceTarget;
}

void USimonGameController::SetCurrentOrbSequenceTarget(TArray<int32> NewTarget)
{
	CurrentOrbSequenceTarget = NewTarget;
}

float USimonGameController::GetBaseOrbFlareDuration() const
{
	return BaseOrbFlareDuration;
}

float USimonGameController::GetGameSpeedIncreaseRate() const
{
	return GameSpeedIncreaseRate;
}

float USimonGameController::GetDifficultyBumpLevels() const
{
	return DifficultyBumpLevels;
}

float USimonGameController::GetCurrentOrbFlareDuration() const
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
			   
	// Remove AActor* from SimonOrbs that are not a SimonOrb to prevent malfunction, connect to Switch Action of the rest.
	SimonOrbs = SimonOrbs.FilterByPredicate([this](const AActor* Actor) {
		USimonOrbController* SimonOrbController = Actor->FindComponentByClass<USimonOrbController>();
		if (!(SimonOrbController))
		{
			UE_LOG(LogTemp, Error, TEXT("%s"), *FString::Printf(TEXT("USimonGameController: USimonOrbController missing on %s! Add a USimonOrbController to it or remove it from the USimonGameController of %s!"), *Actor->GetName(), *this->GetOwner()->GetName()));
			return false;
		}
		else
		{
			SimonOrbController->OnSwitchActivation.AddDynamic(this, &USimonGameController::OnNotificationOfSimonOrbActivation);
			return true;
		}
	});
	   	  
	ResetGame();

	SetSimonGameState(ESimonGameState::PreparingRound);
}

void USimonGameController::ResetGame()
{
	CurrentGameRound = 0;
	CurrentOrbSequenceTarget.Empty();
}

int32 USimonGameController::GetNumberOfSimonOrbs() const
{
	return SimonOrbs.Num();
}


void USimonGameController::FlareOrb(int32 OrbNumber)
{	
	USimonOrbController* SimonOrbController = SimonOrbs[OrbNumber]->FindComponentByClass<USimonOrbController>();
	if (SimonOrbController)
	{
		SimonOrbController->FlareSimonOrb();
	}
}

void USimonGameController::SetAllOrbsPlayerActivatable(bool Activatable)
{
	for (AActor* SimonOrb : SimonOrbs)
	{
		USimonOrbController* SimonOrbController = SimonOrb->FindComponentByClass<USimonOrbController>();
		if (SimonOrbController)
		{
			SimonOrbController->SetPlayerActivatable(Activatable);
		}
	}
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
	return bGameIsWon;
}

void USimonGameController::OnNotificationOfSimonOrbActivation(AActor* ActivatedSimonOrb)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("OnNotificationOfSimonOrbActivation.")));
	for (int32 i = 0; i < SimonOrbs.Num(); ++i)
	{
		if (SimonOrbs[i] == ActivatedSimonOrb)
		{
			CurrentSimonGameState->OnPlayerInput(i);
			break;
		}
	}
}

void USimonGameController::SetSimonGameState(ESimonGameState NewState)
{
	if (CurrentSimonGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Switching State (%d)->(%d)"), CurrentSimonGameState->GetESimonGameState(), NewState));
	}

	delete CurrentSimonGameState;
	CurrentSimonGameState = nullptr;

	switch (NewState)
	{
	case GameLost:
		CurrentSimonGameState = new FGameLost(this);
		CurrentSimonGameState->OnStateEnter();
		break;
	case GameWon:
		CurrentSimonGameState = new FGameWon(this);
		CurrentSimonGameState->OnStateEnter();
		break;
	case AwaitingPlayerInput:
		CurrentSimonGameState = new FAwaitingPlayerInput(this);
		CurrentSimonGameState->OnStateEnter();
		break;
	case DisplayingTargetSequence:
		CurrentSimonGameState = new FDisplayingTargetSequence(this);
		CurrentSimonGameState->OnStateEnter();
		break;
	case PreparingRound:
		CurrentSimonGameState = new FPreparingRound(this);
		CurrentSimonGameState->OnStateEnter();
		break;
	default:
		CurrentSimonGameState = new FGameLost(this);
		CurrentSimonGameState->OnStateEnter();
		break;
	}
}

// Switch to the Target State after a delay. (Delay <= 0.0f) equals no delay.
void USimonGameController::SetSimonGameStateAfterDelay(ESimonGameState NewState, float Delay)
{
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("State Switch in %f"), Delay));
	if (Delay > 0.0f) 
	{
		FTimerHandle TimerHandle{};
		FTimerDelegate TimerDel{};
		TimerDel.BindUFunction(this, FName("SetSimonGameState"), NewState);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, Delay, false);
	} 
	else
	{
		SetSimonGameState(NewState);
	}

}

/* FSimonGameState  Class*/
FSimonGameState::FSimonGameState(const USimonGameController* SimonGameController) : SimonGameState(ESimonGameState::NoState)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("USimonGameState created.")));
	this->SimonGameController = SimonGameController;
}

void FSimonGameState::OnStateEnter()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("USimonGameState OnStateEnter.")));
}

void FSimonGameState::OnStateExit()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("USimonGameState OnStateExit.")));
}

FSimonGameState::~FSimonGameState()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("USimonGameState destroyed.")));
}

void FSimonGameState::OnPlayerInput(int32 OrbNumber)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("USimonGameState OnPlayerInput.")));
}

ESimonGameState FSimonGameState::GetESimonGameState() const
{
	return SimonGameState;
}



/* FSimonGameState UPreparingRound Class*/

ESimonGameState FPreparingRound::GetESimonGameState() const
{
	return ESimonGameState::PreparingRound;
}

void FPreparingRound::OnStateEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("UPreparingRound OnStateEnter.")));

	InitNextRound();
	SimonGameController->SetSimonGameState(ESimonGameState::DisplayingTargetSequence);
}

int32 FPreparingRound::GetRandomOrbNumber()
{
	return FMath::RandRange(0, SimonGameController->GetNumberOfSimonOrbs() - 1);
}

TArray<int32> FPreparingRound::GenerateRandomOrbSequence(int32 SequenceLength)
{
	TArray<int32> OrbSequence{};

	for (int32 i = 0; i < SequenceLength; ++i)
	{
		OrbSequence.Push(GetRandomOrbNumber());
	}

	return OrbSequence;
}

void FPreparingRound::InitNextRound()
{
	SimonGameController->IncrementCurrentGameRound();
	
	uint32 CurrentGameRound = SimonGameController->GetCurrentGameRound();

	if (CurrentGameRound < 1)
	{
		return;
	}


	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Initializing Round - %d"), CurrentGameRound));

	/// old generate sequence - generates a completly new sequence for the round
	//SimonGameController->SetCurrentOrbSequenceTarget ( GenerateRandomOrbSequence(CurrentGameRound) );


	/// new generate sequence - adds to the last sequence

	// 1 + 1 for every X levels ( X = DifficultyBumpLevels)
	int32 NoOfNewTargets = FMath::TruncToInt((CurrentGameRound / SimonGameController->GetDifficultyBumpLevels()) + 1.0f);

	TArray<int32> NewOrbSeq = SimonGameController->GetCurrentOrbSequenceTarget();
	NewOrbSeq.Append(GenerateRandomOrbSequence(NoOfNewTargets));

	SimonGameController->SetCurrentOrbSequenceTarget(NewOrbSeq);

	// set orb flare times
	float FLareDuration = SimonGameController->GetBaseOrbFlareDuration() / (1 + ((CurrentGameRound - 1)  * SimonGameController->GetGameSpeedIncreaseRate()));
	SimonGameController->SetCurrentOrbFlareDuration(FLareDuration);
}



/* FSimonGameState UDisplayingTargetSequence Class*/

ESimonGameState FDisplayingTargetSequence::GetESimonGameState() const
{
	return ESimonGameState::DisplayingTargetSequence;
}

void FDisplayingTargetSequence::OnStateEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("UDisplayingTargetSequence OnStateEnter.")));
	PlayOrbSequence(this->SimonGameController->GetCurrentOrbSequenceTarget());

	// Switch to input state after flare
	SimonGameController->SetSimonGameStateAfterDelay(
		ESimonGameState::AwaitingPlayerInput, 
		(SimonGameController->GetCurrentOrbSequenceTarget().Num()) * SimonGameController->GetCurrentOrbFlareDuration() * 1.05f
	);
}

void FDisplayingTargetSequence::PlayOrbSequence(TArray<int32> OrbSequence)
{
	SimonGameController->FlareOrb(OrbSequence[0]);

	for (int32 i = 1; i < OrbSequence.Num(); ++i)
	{
		FTimerHandle TimerHandle{};
		FTimerDelegate TimerDel{};
		TimerDel.BindUFunction(SimonGameController.Get(), FName("FlareOrb"), OrbSequence[i]);
		SimonGameController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * SimonGameController->GetCurrentOrbFlareDuration() * 1.05f, false);
	}
}



/* FSimonGameState UAwaitingPlayerInput Class*/

FAwaitingPlayerInput::~FAwaitingPlayerInput()
{
	OnStateExit();
}

void FAwaitingPlayerInput::OnStateEnter()
{
	SimonGameController->SetAllOrbsPlayerActivatable(true);
}

void FAwaitingPlayerInput::OnStateExit()
{
	SimonGameController->SetAllOrbsPlayerActivatable(false);
}

void FAwaitingPlayerInput::OnPlayerInput(int32 OrbNumber)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Notification: SimonOrb number %d was activated."), OrbNumber));
	CurrentOrbSequenceInput.Add(OrbNumber);

	ESimonRoundStatus CurrentRoundStatus = CheckRoundStatus();

	switch (CurrentRoundStatus)
	{
	case RoundLost:
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Game Lost in round %d."), SimonGameController->GetCurrentGameRound()));
		//SimonGameController->SetSimonGameState(ESimonGameState::GameLost);
		SimonGameController->SetAllOrbsPlayerActivatable(false);
		SimonGameController->SetSimonGameStateAfterDelay(ESimonGameState::GameLost, SimonGameController->GetCurrentOrbFlareDuration() * 1.05f);
		return;
		break;
	case ContinueRound:
		break;
	case RoundWon:

		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Round Won.")));
		if (SimonGameController->GetCurrentGameRound() == SimonGameController->GetGameRoundsToWin())
		{

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Game won after %d rounds."), SimonGameController->GetCurrentGameRound()));
			//SimonGameController->SetSimonGameState(ESimonGameState::GameWon);
			SimonGameController->SetAllOrbsPlayerActivatable(false);
			SimonGameController->SetSimonGameStateAfterDelay(ESimonGameState::GameWon, SimonGameController->GetCurrentOrbFlareDuration() * 1.05f);
			return;
		}
		else
		{
			//SimonGameController->SetSimonGameState(ESimonGameState::PreparingRound);
			SimonGameController->SetAllOrbsPlayerActivatable(false);
			SimonGameController->SetSimonGameStateAfterDelay(ESimonGameState::PreparingRound, SimonGameController->GetCurrentOrbFlareDuration() * 1.05f);
			return;
		}
		break;
	default:
		break;
	}
}


ESimonRoundStatus FAwaitingPlayerInput::CheckRoundStatus()
{
	if (!IsInputSequenceOkay())
	{
		return ESimonRoundStatus::RoundLost;
	}
	else if (CurrentOrbSequenceInput.Num() == SimonGameController->GetCurrentOrbSequenceTarget().Num())
	{
		return ESimonRoundStatus::RoundWon;
	}
	return ESimonRoundStatus::ContinueRound;
}


bool FAwaitingPlayerInput::IsInputSequenceOkay()
{
	auto CurrentOrbSequenceTarget = SimonGameController->GetCurrentOrbSequenceTarget();
	for (int32 i = 0; i < CurrentOrbSequenceInput.Num(); ++i) {
		if (CurrentOrbSequenceInput[i] !=  CurrentOrbSequenceTarget[i])
		{
			return false;
		}
	}
	return true;
}


/* FSimonGameState UGameLost Class*/

void FGameLost::OnStateEnter()
{
	SimonGameController->ResetGame();
	SimonGameController->SetSimonGameState(ESimonGameState::PreparingRound);
}


/* FSimonGameState UWaiting Class*/

void FWaiting::OnStateEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Waiting OnStateEnter.")));
}

void FWaiting::OnStateExit()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Waiting OnStateExit.")));
}
