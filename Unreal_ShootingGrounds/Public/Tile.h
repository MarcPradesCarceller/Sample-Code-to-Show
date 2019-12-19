// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UActorPool;

UCLASS()
class SHOOTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void PlaceActors(TSubclassOf<AActor> toSpawn, int minSpawn, int maxSpawn, FVector size);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void PlaceAIPawns(TSubclassOf<APawn> toSpawn, int minSpawn, int maxSpawn, FVector size);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool *inPool);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	FVector minObjectSpawnExtend;
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	FVector maxObjectSpawnExtend;

	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	FVector minAISpawnExtend;
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	FVector maxAISpawnExtend;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector navigationBoundsOffset;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	const int MAX_ATTEMPTS = 40;

	const FVector MIN_SPAWNOBJECT_BOXAREA = FVector(18.f, -1000.f, 300.f);
	const FVector MAX_SPAWNOBJECT_BOXAREA = FVector(3260.0f, 1000.f, 300.f);

	const FVector MIN_SPAWNENEMY_BOXAREA = FVector(612.f, -495.f, 45.f);
	const FVector MAX_SPAWNENEMY_BOXAREA = FVector(3157.f, 495.f, 45.f);

	const FVector NAVMESH_BOUNDS_OFFSET = FVector(1668.f, 0.f, 110.f);

	UActorPool *pool;
	AActor *navMeshBoundsVolume;

	//Finds a empty location on the new tile
	bool FindEmptyLocation(FVector &outLocation, bool isIA, FVector size, float rotation);

	//Method to create, spawn and place an object on the map
	void PlaceActor(TSubclassOf<AActor> toSpawn, FVector point, float rotation);

	//Method to create, spawn and place an enemy on the map
	void PlaceIAPawn(TSubclassOf<APawn> toSpawn, FVector point, float rotation);

	//Checks if the volume space collides with other element
	bool CanSpawnAtLocation(FVector location, bool isAI, FVector size, float rotation);

	//Puts the Actor to ground level
	FVector GroundObjects(FVector location, FVector size, float rotation);
};
