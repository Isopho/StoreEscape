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

			//SimonOrbController->RegisterDelegateToSwitchActivation(this, &USimonGameController::OnNotificationOfSimonOrbActivation);
		}
	}

	// Prepare first game.
	//while (!GameWon) {
		// prepare new game
		ResetGame();

		//start new round
		++CurrentGameRound;

		if (CurrentGameRound > GameRounds)
		{
			//(no more rounds to play->win(play win animation))
		}


		//generate sequence
		TArray<int32> CurrentOrbSequence = GenerateRandomOrbSequence(CurrentGameRound);


		// play sequence
		// set orb flare times
		float FlareTimePerOrb = 20.0f / (9 + (CurrentGameRound * BaseGameSpeed));
		for (AActor* SimonOrb : SimonOrbs) 
		{
			USimonOrbController* SimonOrbController = SimonOrb->FindComponentByClass<USimonOrbController>();
			if (SimonOrbController)
			{
				SimonOrbController->SetFlareTime(FlareTimePerOrb);
				SimonOrbController->SetPlayerActivatable(true);
			}
		}

		PlayOrbSequence(CurrentOrbSequence, FlareTimePerOrb);
		
	


		// wait X* seconds for completion

		// if no input reset()

		// if input 
			//play orb input
			//if input correct 
				//if sequence complete continue to next round & play rounde complete animation
				//else wait for more input
	//}



	
}

void USimonGameController::ResetGame()
{
	CurrentGameRound = 5;
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
	for (int32 i = 0; i < OrbSequence.Num(); ++i) 
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
	UE_LOG(LogTemp, Warning, TEXT("NotifyOfOrbActivation!"));
	for (int32 i = 0; i < SimonOrbs.Num(); ++i)
	{
		if (SimonOrbs[i] == ActivatedSimonOrb)
		{
			// do something with i
			

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Notification: SimonOrb number %d was activated."), i));
		}

	}
}




