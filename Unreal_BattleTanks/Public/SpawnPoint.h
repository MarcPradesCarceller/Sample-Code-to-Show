// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpawnPoint.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANKS_API USpawnPoint : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USpawnPoint();

	AActor *GetSpawnedActor() const { return spawnedActor; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AActor> spawnClass;

	UPROPERTY()
	AActor *spawnedActor;
};
