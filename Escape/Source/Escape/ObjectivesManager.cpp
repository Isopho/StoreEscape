// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectivesManager.h"

// Sets default values for this component's properties
UObjectivesManager::UObjectivesManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UObjectivesManager::BeginPlay()
{
	Super::BeginPlay();	
}


// Called every frame
void UObjectivesManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UObjectivesManager::AddObjective(FString ObjectiveName, FString ObjectiveText)
{
	if (!ObjectivesMap.Contains(ObjectiveName))
	{
		ObjectivesMap.Add(ObjectiveName, ObjectiveText);

		OnAddObjectiveToHUD.Broadcast(ObjectiveName, ObjectiveText);
	}
	else
	{
		OnUpdateObjectiveInHUD.Broadcast(ObjectiveName, ObjectiveText);
	}

}

FString UObjectivesManager::GetObjective(FString ObjectiveName)
{
	if (ObjectivesMap.Contains(ObjectiveName))
	{
		return ObjectivesMap[ObjectiveName];
	}
	else
	{
		return "";
	}
}

void UObjectivesManager::RemoveObjective(FString ObjectiveName)
{
	if (ObjectivesMap.Contains(ObjectiveName))
	{
		ObjectivesMap.Remove(ObjectiveName);
		OnRemoveObjectiveFromHUD.Broadcast(ObjectiveName);
	}
}

