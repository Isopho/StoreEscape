// Copyright Jan-Uriel Lorbeer 2019.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActivationReceiver.generated.h"

/**
* Specialized ActorComponent that can be inherited so that
* "Activator" can call DoActivationAction on Actor.
* Activation possible by click or line-trace.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPE_API UActivationReceiver : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActivationReceiver();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		virtual void OnClick();

	// Action to be done when activated.
	virtual void DoActivationAction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
