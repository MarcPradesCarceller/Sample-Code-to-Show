// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "SpringWheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
    Super::BeginPlay();
}

TArray<ASpringWheel *> UTankTrack::GetWheels() const
{
    TArray<ASpringWheel *> resultArray;
    TArray<USceneComponent *> children;
    GetChildrenComponents(true, children);

    for (USceneComponent *child : children)
    {
        auto spawnPointChild = Cast<USpawnPoint>(child);
        if (!spawnPointChild)
            continue;

        AActor *spawnedChild = spawnPointChild->GetSpawnedActor();

        auto springWheel = Cast<ASpringWheel>(spawnedChild);
        if (!springWheel)
            continue;

        resultArray.Add(springWheel);
    }

    return resultArray;
}

void UTankTrack::SetThrottle(float throttle)
{
    float currentThrottle = FMath::Clamp<float>(throttle, -1.0f, 1.0f);
    DriveTrack(currentThrottle);
}

void UTankTrack::DriveTrack(float throttle)
{
    auto forceApplied = throttle * trackMaxDrivingForce;
    auto wheelsArray = GetWheels();
    auto forcePerWheel = forceApplied / wheelsArray.Num();

    for (ASpringWheel *wheelObj : wheelsArray)
    {
        wheelObj->AddDrivingForce(forcePerWheel);
    }
}
