// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

AActor *UActorPool::Checkout()
{
	if (pool.Num() == 0)
		return nullptr;

	return pool.Pop();
}

void UActorPool::Return(AActor *actorToReturn)
{
	if (actorToReturn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("The Tile had no NavMesh asociated with the pool"));
		return;
	}
	pool.Push(actorToReturn);
}

void UActorPool::Add(AActor *actorToAdd)
{
	if (actorToAdd == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("The actor to add will be null"));
		return;
	}
	pool.Push(actorToAdd);
}