// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootingGroundsCharacter.h"
#include "ShootingGroundsProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Gun.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);
AShootingGroundsCharacter::AShootingGroundsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

void AShootingGroundsCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();
	Mesh1P->SetHiddenInGame(false, true);

	//SetupPlayerInputComponent is called before BeginPlay, and the Fire BindActions need a reference to the player gun
	//So the player gun is set up in SetupPlayerInputComponent
}

void AShootingGroundsCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	playerGun->Destroy();
}

void AShootingGroundsCharacter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	if (gunBlueprint != NULL)
	{
		if (playerGun == NULL)
		{
			playerGun = GetWorld()->SpawnActor<AGun>(gunBlueprint);
			playerGun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			playerGun->animInstanceFP = Mesh1P->GetAnimInstance();
		}
	}

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShootingGroundsCharacter::Fire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AShootingGroundsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShootingGroundsCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShootingGroundsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShootingGroundsCharacter::LookUpAtRate);
}

void AShootingGroundsCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AShootingGroundsCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AShootingGroundsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShootingGroundsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShootingGroundsCharacter::Fire()
{
	playerGun->OnFire();
}
