// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h "
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimComponent::Initialice(UTankBarrel *barrelToSet, UTankTurret *turretToSet)
{
	barrel = barrelToSet;
	turret = turretToSet;
}

void UTankAimComponent::SetProjectileBlueprint(TSubclassOf<AProjectile> projectile)
{
	projectileBlueprint = projectile;
}

void UTankAimComponent::BeginPlay()
{
	Super::BeginPlay();
	lastFireTime = FPlatformTime::Seconds();
}

void UTankAimComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (roundsLeft <= 0)
	{
		firingState = EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - lastFireTime) < reloadTime)
	{
		firingState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		firingState = EFiringState::Aiming;
	}
	else
	{
		firingState = EFiringState::Locked;
	}
}

void UTankAimComponent::AimAt(FVector aimPosition)
{

	if (!barrel || !turret)
		return;

	FVector outLaunchVelocity;
	FVector startLocation = barrel->GetSocketLocation(FName("Projectile"));

	if (UGameplayStatics::SuggestProjectileVelocity(this, outLaunchVelocity, startLocation, aimPosition, launchSpeed, false, 0.0f, 0.0f, ESuggestProjVelocityTraceOption::DoNotTrace))
	{
		aimDirection = outLaunchVelocity.GetSafeNormal();

		MoveBarrelTowards(aimDirection);
	}
}

void UTankAimComponent::MoveBarrelTowards(FVector aimDirection)
{
	if (!barrel || !turret)
		return;
	auto barrelRotation = barrel->GetForwardVector().Rotation();
	auto aimRotation = aimDirection.Rotation();

	auto deltaRotator = aimRotation - barrelRotation;

	barrel->Elevate(deltaRotator.Pitch);

	if (FMath::Abs(deltaRotator.Yaw) < 180)
	{
		turret->Rotate(deltaRotator.Yaw);
	}
	else
	{
		turret->Rotate(-deltaRotator.Yaw);
	}
}

void UTankAimComponent::Fire()
{

	if (firingState == EFiringState::Locked || firingState == EFiringState::Aiming)
	{

		if (barrel && projectileBlueprint)
		{
			auto proj = GetWorld()->SpawnActor<AProjectile>(projectileBlueprint, barrel->GetSocketLocation("Projectile"), barrel->GetSocketRotation("Projectile"));
			proj->LaunchProjectile(launchSpeed);

			lastFireTime = FPlatformTime::Seconds();
			roundsLeft--;
		}
	}
}

bool UTankAimComponent::IsBarrelMoving()
{

	if (!barrel)
		return false;

	auto barrelForward = barrel->GetForwardVector();
	return barrelForward.Equals(aimDirection, 0.05f);
}

EFiringState UTankAimComponent::GetFiringState() const
{
	return firingState;
}

int32 UTankAimComponent::GetRoundsLeft() const
{
	return roundsLeft;
}