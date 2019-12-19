// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Animation/AnimInstance.h"
#include "ShootingGroundsProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a Gun mesh component
	gunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	gunMesh->bCastDynamicShadow = false;
	gunMesh->CastShadow = false;
	// gunMesh->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	gunMesh->SetupAttachment(RootComponent);

	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(gunMesh);
	muzzleLocation->SetRelativeLocation(FVector(0.2f, 61.4f, 11.6f));
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::OnFire()
{
	// try and fire a projectile
	if (projectileClass != NULL)
	{
		UWorld *const world = GetWorld();
		if (world != NULL)
		{

			const FRotator spawnRotation = muzzleLocation->GetComponentRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector spawnLocation = muzzleLocation->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters actorSpawnParams;
			actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			world->SpawnActor<AShootingGroundsProjectile>(projectileClass, spawnLocation, spawnRotation, actorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (fireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (fireAnimationFP != NULL)
	{
		if (animInstanceFP != NULL)
		{
			animInstanceFP->Montage_Play(fireAnimationFP, 1.f);
		}
	}
	if (fireAnimationTP != NULL)
	{
		if (animInstanceTP != NULL)
		{
			animInstanceTP->Montage_Play(fireAnimationTP, 1.f);
		}
	}
}
