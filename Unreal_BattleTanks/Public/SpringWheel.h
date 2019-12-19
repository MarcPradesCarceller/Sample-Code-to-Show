// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpringWheel.generated.h"

class UPhysicsConstraintComponent;
class USphereComponent;

UCLASS()
class BATTLETANKS_API ASpringWheel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpringWheel();
	virtual void Tick(float DeltaTime) override;

	void AddDrivingForce(float forceMagnitude);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent *wheel = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent *axle = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent *massWheelConstraint = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent *axleWheelConstraint = nullptr;

	float totalForceMagnitudeThisFrame = 0;

	void SetupConstraint();

	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	void ApplyForce();
};
