// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTINGGROUNDS_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActorPool();

public:
	//Gives the actor of the pool
	AActor *Checkout();
	//Returns the actor to the pool
	void Return(AActor *actorToReturn);

	void Add(AActor *actorToAdd);

private:
	TArray<AActor *> pool;
};
