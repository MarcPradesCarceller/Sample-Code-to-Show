// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
    navMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("NavMeshBoundsPool"));
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
    TActorIterator<ANavMeshBoundsVolume> volumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

    while (volumeIterator)
    {
        AddToPool(*volumeIterator);
        ++volumeIterator;
    }
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume *volumeToAdd)
{
    navMeshBoundsVolumePool->Add(volumeToAdd);
}
