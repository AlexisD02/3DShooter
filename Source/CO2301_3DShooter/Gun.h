// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "Gun.generated.h"

UCLASS()
class CO2301_3DSHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	AGun();

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void EmptyMug();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* WeaponGunMesh;

private:
	UPROPERTY(EditAnywhere)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
	USoundBase* EmptyMugSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
};
