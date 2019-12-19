// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimComponent.h"
#include "Tank.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{

	Super::BeginPlay();
	if (!GetPawn())
		return;
	aimingComponent = GetPawn()->FindComponentByClass<UTankAimComponent>();
	if (aimingComponent)
		FoundAimingComponent(aimingComponent);
	else
		UE_LOG(LogTemp, Warning, TEXT("Missing Aiming Component"));
}

void ATankPlayerController::SetPawn(APawn *InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto possessedTank = Cast<ATank>(InPawn);
		if (!ensure(possessedTank))
			return;
		possessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{

	if (!GetPawn())
		return;

	if (!aimingComponent)
		aimingComponent = GetPawn()->FindComponentByClass<UTankAimComponent>();

	FVector hitLocation; //OutParameter
	if (GetSightRayHitLocation(hitLocation))
	{
		aimingComponent->AimAt(hitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &outHitLocation) const
{

	int viewportSizeX, viewportSizeY;

	GetViewportSize(viewportSizeX, viewportSizeY);

	FVector2D screenReticle = FVector2D(viewportSizeX * screenPositionXPertentage, viewportSizeY * screenPositionYPertentage);

	FVector lookDirection;
	if (GetLookDirection(screenReticle, lookDirection))
	{
		return GetLookVectorHitLocation(lookDirection, outHitLocation);
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D screenLocation, FVector &lookDirection) const
{

	FVector cameraWorldLocation;
	return DeprojectScreenPositionToWorld(screenLocation.X, screenLocation.Y, cameraWorldLocation, lookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector lookDirection, FVector &hitLocation) const
{

	FHitResult hitResult;
	auto startLocation = PlayerCameraManager->GetCameraLocation();
	auto endLocation = startLocation + lookDirection * 1000000;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Camera))
	{

		hitLocation = hitResult.Location;
		return true;
	}
	hitLocation = FVector(0);
	return false;
}

void ATankPlayerController::OnTankDeath()
{
	StartSpectatingOnly();
}