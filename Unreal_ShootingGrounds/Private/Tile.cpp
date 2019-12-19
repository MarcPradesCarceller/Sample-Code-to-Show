// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "ActorPool.h"
#include "NavigationSystem.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	minObjectSpawnExtend = MIN_SPAWNOBJECT_BOXAREA;
	maxObjectSpawnExtend = MAX_SPAWNOBJECT_BOXAREA;

	minAISpawnExtend = MIN_SPAWNENEMY_BOXAREA;
	maxAISpawnExtend = MAX_SPAWNENEMY_BOXAREA;

	navigationBoundsOffset = NAVMESH_BOUNDS_OFFSET;
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (pool != nullptr && navMeshBoundsVolume != nullptr)
	{
		pool->Return(navMeshBoundsVolume);
	}
}

//Start method, called from build blueprint
void ATile::PlaceActors(TSubclassOf<AActor> toSpawn, int minSpawn, int maxSpawn, FVector size)
{

	int numberToSpawn = FMath::RandRange(minSpawn, maxSpawn);
	FVector spawnPoint;
	float rotation;
	for (size_t i = 0; i < numberToSpawn; i++)
	{
		rotation = FMath::RandRange(-180.f, 180.f);
		if (FindEmptyLocation(spawnPoint, false, size, rotation))
		{
			spawnPoint = GroundObjects(spawnPoint, size, rotation);
			PlaceActor(toSpawn, spawnPoint, rotation);
		}
	}
}
//Called to Spawn Enemyes
void ATile::PlaceAIPawns(TSubclassOf<APawn> toSpawn, int minSpawn, int maxSpawn, FVector size)
{
	int numberToSpawn = FMath::RandRange(minSpawn, maxSpawn);
	FVector spawnPoint;
	float rotation;
	for (size_t i = 0; i < numberToSpawn; i++)
	{
		rotation = FMath::RandRange(-180.f, 180.f);
		if (FindEmptyLocation(spawnPoint, true, size, rotation))
		{
			PlaceIAPawn(toSpawn, spawnPoint, rotation);
		}
	}
}

//Random checks to find an empty place where to spawn
bool ATile::FindEmptyLocation(FVector &outLocation, bool isIA, FVector size, float rotation)
{
	FVector candidate;

	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{

		if (isIA)
			candidate = FMath::RandPointInBox(FBox(minAISpawnExtend, maxAISpawnExtend));
		else
			candidate = FMath::RandPointInBox(FBox(minObjectSpawnExtend, maxObjectSpawnExtend));

		if (CanSpawnAtLocation(candidate, isIA, size, rotation))
		{
			outLocation = candidate;
			return true;
		}
	}

	return false;
}
//Spawns the object
void ATile::PlaceActor(TSubclassOf<AActor> toSpawn, FVector point, float rotation)
{

	AActor *spawned = GetWorld()->SpawnActor<AActor>(toSpawn);
	spawned->SetActorRelativeLocation(point);
	spawned->SetActorRotation(FRotator(0, rotation, 0));
	spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	spawned->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);
}

//Spawns the enemy
void ATile::PlaceIAPawn(TSubclassOf<APawn> toSpawn, FVector point, float rotation)
{
	APawn *spawned = GetWorld()->SpawnActor<APawn>(toSpawn);
	spawned->SetActorRelativeLocation(point);
	spawned->SetActorRotation(FRotator(0, rotation, 0));
	spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	spawned->SpawnDefaultController();
	spawned->Tags.Add(FName("Enemy"));
}

//Checks if the volume space collides with other element
bool ATile::CanSpawnAtLocation(FVector location, bool isAI, FVector size, float rotation)
{

	FHitResult hitResult;
	FVector globalLocation = ActorToWorld().TransformPosition(location);
	FVector boxCenter;

	if (isAI)
		boxCenter = globalLocation;
	else
		boxCenter = globalLocation + FVector(0, 0, size.Z);

	bool hasHit = GetWorld()->SweepSingleByChannel(hitResult, boxCenter, boxCenter + FVector(0, 0, .01), FQuat::MakeFromEuler(FVector(0, 0, rotation)), ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeBox(size));

	return !hasHit;
}

//Puts the Actor to ground level
FVector ATile::GroundObjects(FVector location, FVector size, float rotation)
{

	FHitResult hitResult;
	FVector globalLocation = ActorToWorld().TransformPosition(location);
	FVector boxCenter = globalLocation + FVector(0, 0, size.Z);

	bool hasHit = GetWorld()->SweepSingleByChannel(hitResult, boxCenter, boxCenter + FVector(0, 0, -500), FQuat::MakeFromEuler(FVector(0, 0, rotation)), ECollisionChannel::ECC_Camera, FCollisionShape::MakeBox(size));

	if (hasHit)
	{
		return FVector(location.X, location.Y, location.Z - hitResult.Distance);
	}
	else
		return location;
}

void ATile::SetPool(UActorPool *inPool)
{
	pool = inPool;

	navMeshBoundsVolume = pool->Checkout();
	if (navMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Not Enough actors in the pool"));

		return;
	}
	navMeshBoundsVolume->SetActorLocation(GetActorLocation() + navigationBoundsOffset);
	FNavigationSystem::Build(*GetWorld());
}