// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (gunBlueprint == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun blueprint missing."));
		return;
	}
	if (gun == NULL && enemyMesh != NULL)
	{
		gun = GetWorld()->SpawnActor<AGun>(gunBlueprint);
		gun->AttachToComponent(enemyMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		gun->animInstanceTP = enemyMesh->GetAnimInstance();
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::Fire()
{
	gun->OnFire();
}
