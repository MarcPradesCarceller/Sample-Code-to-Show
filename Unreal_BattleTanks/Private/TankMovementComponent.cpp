// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack *leftTrack, UTankTrack *rightTrack)
{
    if (!leftTrack || !rightTrack)
        return;

    trackL = leftTrack;
    trackR = rightTrack;
}

void UTankMovementComponent::RequestDirectMove(const FVector &moveVelocity, bool bForceMaxSpeed)
{
    //UE_LOG(LogTemp, Warning, TEXT("Calculating Path"));

    auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
    auto AIForwardIntention = moveVelocity.GetSafeNormal();

    auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
    IntendMoveForward(ForwardThrow);

    auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
    IntendTurnRight(RightThrow);
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    if (!trackL || !trackR)
        return;
    trackL->SetThrottle(Throw);
    trackR->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
    if (!trackL || !trackR)
        return;
    trackL->SetThrottle(Throw);
    trackR->SetThrottle(-Throw);
}
