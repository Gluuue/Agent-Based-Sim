// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Agent.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

	void Init(UWorld* world, UStaticMeshComponent* mesh);
	void Flock();

	FVector MoveTowardsFlockCenter(AAgent* boid);
	FVector PersonalSpace(AAgent* boid);
	FVector MatchSpeed(AAgent* boid);
	void LimitSpeed(AAgent* boid);
	FVector BoundPos(AAgent* boid);

private:
	UWorld* World;
	bool initialized;
	TArray<class AAgent*> Agents;
};
