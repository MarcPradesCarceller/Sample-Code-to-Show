// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimComponent;

UCLASS()
class BATTLETANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimComponent *aimComponentRef);

	UFUNCTION()
	void OnTankDeath();

private:
	UTankAimComponent *aimingComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float screenPositionXPertentage = 0.5f;
	UPROPERTY(EditAnywhere)
	float screenPositionYPertentage = 0.33f;

	virtual void BeginPlay() override;
	void AimTowardsCrosshair();
	virtual void Tick(float DeltaTime) override;
	bool GetSightRayHitLocation(FVector &outHitLocation) const;
	bool GetLookDirection(FVector2D screenLocation, FVector &lookDirection) const;

	bool GetLookVectorHitLocation(FVector lookDirection, FVector &hitLocation) const;

	virtual void SetPawn(APawn *InPawn) override;

};
