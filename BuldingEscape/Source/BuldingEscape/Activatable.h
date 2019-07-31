// Copyright Jan-Uriel Lorbeer 2019.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Activatable.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BULDINGESCAPE_API UActivatable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActivatable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateActivatable();
		
};
