// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

class ASpringWheel;

UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANKS_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetThrottle(float throttle);

	UPROPERTY(EditDefaultsOnly)
	float trackMaxDrivingForce = 4500000;

private:
	TArray<ASpringWheel *> GetWheels() const;

	UTankTrack();
	virtual void BeginPlay() override;
	void DriveTrack(float throttle);
};
