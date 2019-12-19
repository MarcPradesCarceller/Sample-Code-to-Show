// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimComponent.generated.h"

UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANKS_API UTankAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimComponent();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialice(UTankBarrel *barrelToSet, UTankTurret *turretToSet);
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetProjectileBlueprint(TSubclassOf<AProjectile> projectile);

	void AimAt(FVector aimPosition);

	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();

	EFiringState GetFiringState() const;

	UFUNCTION(BlueprintCallable, Category = Firing)
	int32 GetRoundsLeft() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	EFiringState firingState = EFiringState::Reloading;

private:
	UTankBarrel *barrel = nullptr;
	UTankTurret *turret = nullptr;

	FVector aimDirection;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float launchSpeed = 10000;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> projectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	int32 roundsLeft = 20;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float reloadTime = 3;

	float lastFireTime = 0;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void MoveBarrelTowards(FVector aimDirection);
	bool IsBarrelMoving();
};
