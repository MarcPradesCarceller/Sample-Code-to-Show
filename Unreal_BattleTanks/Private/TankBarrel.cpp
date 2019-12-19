// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"

void UTankBarrel::Elevate(float relativeSpeed)
{

	auto elevationDif = FMath::Clamp(relativeSpeed, -1.0f, 1.0f) * maxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto newRotation = FMath::Clamp(RelativeRotation.Pitch + elevationDif, minElevation, maxElevation);

	SetRelativeRotation(FRotator(newRotation, 0, 0));
}