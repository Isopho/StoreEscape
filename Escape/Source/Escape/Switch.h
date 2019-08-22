// Copyright Jan-Uriel Lorbeer 2019.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActivationReceiver.h"
#include "Switch.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchActivation, AActor*, ActivatedSwtichActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API USwitch : public UActivationReceiver
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwitch();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	   
	UPROPERTY(BlueprintAssignable)
		FOnSwitchActivation OnSwitchActivation;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void DoActivationAction();

		
};
