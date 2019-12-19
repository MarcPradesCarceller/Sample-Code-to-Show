// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class AGun;

UCLASS()
class SHOOTINGGROUNDS_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AGun> gunBlueprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	class USkeletalMeshComponent *enemyMesh;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AlloqPrivateAccess = "true"))
	AGun *gun;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
