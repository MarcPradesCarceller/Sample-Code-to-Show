// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAIController.h"
#include "TankAimComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

void ATankAIController::BeginPlay()
{

	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn *InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto possessedTank = Cast<ATank>(InPawn);
		if (!possessedTank)
			return;
		possessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
	}
}

void ATankAIController::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	auto playerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto controlledTank = GetPawn();

	if (!(playerTank && controlledTank))
		return;

	if (!aimComponent)
		aimComponent = controlledTank->FindComponentByClass<UTankAimComponent>();

	MoveToActor(playerTank, closeRangeRadius);

	aimComponent->AimAt(playerTank->GetActorLocation());

	if (aimComponent->GetFiringState() == EFiringState::Locked)
	{
		aimComponent->Fire();
	}
}

void ATankAIController::OnTankDeath()
{
	if (!GetPawn())
		return;
	GetPawn()->DetachFromControllerPendingDestroy();
}
