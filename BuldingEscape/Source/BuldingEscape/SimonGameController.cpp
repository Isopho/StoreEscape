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

	// Prepare first game.
	ResetGame();

	
}

void USimonGameController::ResetGame()
{
	CurrentGameRound = 1;
}

TArray<int32> USimonGameController::GenerateRandomOrbSequence(int32 SequenceLength)
{
	TArray<int32> OrbSequence{};

	for (int32 i = 0; i < SequenceLength; ++i)
	{
		OrbSequence.Push(FMath::RandRange(0, SimonOrbs.Num()));
	}

	return OrbSequence;
}

// Returns if the game round was won.
bool USimonGameController::StartGameRound()
{
	TArray<int32> OrbSequence{ GenerateRandomOrbSequence(CurrentGameRound) };

	// The time each orb glows in seconds.
	float TimePerOrb = 20.0f / (9 + (CurrentGameRound * BaseGameSpeed));



	return false;
}


// Called every frame
void USimonGameController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// check if Game had ....
}

bool USimonGameController::IsGameWon() const
{
	return GameWon;
}

