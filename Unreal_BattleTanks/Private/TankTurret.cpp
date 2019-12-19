// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"

void UTankTurret::Rotate(float relativeSpeed)
{

	auto rotationDif = FMath::Clamp(relativeSpeed, -1.0f, 1.0f) * maxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto newRotation = RelativeRotation.Yaw + rotationDif;

	SetRelativeRotation(FRotator(0, newRotation, 0));
}
