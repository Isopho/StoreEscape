// Copyright Jan-Uriel Lorbeer 2019.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Engine/TriggerVolume.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "OpenDoor.generated.h"


/// Class for blueprint to open the door.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OpenRequest;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent CloseRequest;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float StartDegree{};

	UPROPERTY(EditAnywhere)
		float TriggerMass = 10.0f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate {};

	// Returns total mass in kg.
	float TotalMassOfActorsOnTriggerArea();

	void OpenDoor();

	void CloseDoor();
};
