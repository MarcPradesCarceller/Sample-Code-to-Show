// Fill out your copyright notice in the Description page of Project Settings.

#include "SpringWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASprungWheel::ASpringWheel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.TickGroup = TG_PostPhysics;

	massWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheel Constraint"));
	SetRootComponent(massWheelConstraint);

	axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	axle->AttachToComponent(massWheelConstraint, FAttachmentTransformRules::KeepRelativeTransform);

	wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	wheel->AttachToComponent(axle, FAttachmentTransformRules::KeepRelativeTransform);

	axleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheel Constraint"));
	axleWheelConstraint->AttachToComponent(axle, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ASpringWheel::BeginPlay()
{
	Super::BeginPlay();

	wheel->SetNotifyRigidBodyCollision(true);
	wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

	SetupConstraint();
}

// Called every frame
void ASpringWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		totalForceMagnitudeThisFrame = 0;
	}
}

void ASpringWheel::SetupConstraint()
{
	if (!GetAttachParentActor())
		return;

	UPrimitiveComponent *bodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	if (!bodyRoot)
		return;

	Cast<UPhysicsConstraintComponent>(massWheelConstraint)->SetConstrainedComponents(bodyRoot, NAME_None, axle, NAME_None);
	Cast<UPhysicsConstraintComponent>(axleWheelConstraint)->SetConstrainedComponents(axle, NAME_None, wheel, NAME_None);
}

void ASpringWheel::AddDrivingForce(float forceMagnitude)
{
	totalForceMagnitudeThisFrame += forceMagnitude;
}

//Only apply force when the wheels are colliding with something
void ASpringWheel::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	ApplyForce();
}

void ASpringWheel::ApplyForce()
{
	wheel->AddForce(axle->GetForwardVector() * totalForceMagnitudeThisFrame);
}