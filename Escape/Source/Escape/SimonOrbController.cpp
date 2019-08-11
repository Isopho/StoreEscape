// Copyright Jan-Uriel Lorbeer 2019.


#include "SimonOrbController.h"

// Sets default values for this component's properties
USimonOrbController::USimonOrbController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USimonOrbController::DoActivationAction()
{
	if (bPlayerActivatable && !bActivationBlocked) {
		bActivationBlocked = true;
		FlareSimonOrb();

		OnSwitchActivation.Broadcast(GetOwner());

		FTimerHandle TimerHandle{};
		FTimerDelegate TimerDel{};
		TimerDel.BindUFunction(this, FName("SetActivationBlocked"), false);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, FlareTime, false);
	}
}

void USimonOrbController::SetFlareTime(float FlareTime)
{
	this->FlareTime = FlareTime;
}

float USimonOrbController::GetFlareTime() const
{
	return FlareTime;
}

void USimonOrbController::SetFlareLightIntensity(float FlareLightIntensity)
{
	this->FlareLightIntensity = FlareLightIntensity;
}

float USimonOrbController::GetFlareLightIntensity() const
{
	return FlareLightIntensity;
}

void USimonOrbController::SetPlayerActivatable(bool PlayerActivatable)
{
	this->bPlayerActivatable = PlayerActivatable;
}

void USimonOrbController::SetActivationBlocked(bool ActivationBlocked)
{
	this->bActivationBlocked = ActivationBlocked;
}

bool USimonOrbController::IsPlayerActivatable() const
{
	return bPlayerActivatable;
}

// Called when the game starts
void USimonOrbController::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void USimonOrbController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USimonOrbController::FlareSimonOrb(float VolumeMultiplyer) const
{
	if (VolumeMultiplyer < 0.01f) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("Flaring Orb at (%f)"), VolumeMultiplyer));
	}
	FlareOrbRequest.Broadcast(FlareTime,FlareLightIntensity, VolumeMultiplyer);

}

void USimonOrbController::FlareSimonOrb(float Duration, float LightIntensity, float VolumeMultiplyer)
{
	this->FlareTime = Duration;
	this->FlareLightIntensity = LightIntensity;
	FlareSimonOrb(VolumeMultiplyer);
}

void USimonOrbController::SetSimonOrbGlow(bool Glowing)
{
	bIsGlowing = Glowing;

	if (Glowing) {
		StartOrbGlowRequest.Broadcast(GlowLoopDuration);
	}
	else
	{
		StopOrbGlowRequest.Broadcast();
	}
}

