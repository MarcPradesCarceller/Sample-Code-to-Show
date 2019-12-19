// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SHOOTINGGROUNDS_API AGun : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent *gunMesh;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	class USceneComponent *muzzleLocation;

public:
	// Sets default values for this actor's properties
	AGun();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AShootingGroundsProjectile> projectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase *fireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage *fireAnimationFP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage *fireAnimationTP;

	UPROPERTY()
	class UAnimInstance *animInstanceFP;

	UPROPERTY()
	class UAnimInstance *animInstanceTP;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
