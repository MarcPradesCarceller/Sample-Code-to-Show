// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "Projectile.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	currentHealt = startingHealth;
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	int32 damagePoints = FPlatformMath::RoundToInt(DamageAmount); //Damage is calculated in int32 to avoid float point comparsions
	int32 damageToApply = FMath::Clamp(damagePoints, 0, currentHealt);

	currentHealt -= damageToApply;
	if (currentHealt <= 0)
	{
		OnDeath.Broadcast();
	}

	return damageToApply;
}

float ATank::GetHealthPercent() const
{
	return (float)currentHealt / (float)startingHealth;
}