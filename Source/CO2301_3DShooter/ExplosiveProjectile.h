// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"

#include "ExplosiveProjectile.generated.h"

UCLASS()
class CO2301_3DSHOOTER_API AExplosiveProjectile : public AActor
{
	GENERATED_BODY()
	
public:
    AExplosiveProjectile(); // Sets default values for this actor's properties

protected:
    virtual void BeginPlay() override; // Called when the game starts or when spawned

public:
    UPROPERTY(EditAnywhere)
    float ExplosionDelay = 3.0f;

    UPROPERTY(EditAnywhere)
    float ExplosionDamage = 80.0f;

    UPROPERTY(EditAnywhere)
    float DamageRadius = 350.0f;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(VisibleAnywhere)
    URadialForceComponent* BlastForceComponent;

    UPROPERTY(EditAnywhere)
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditAnywhere)
    USoundBase* ExplosionSound;

    FTimerHandle ExplosionTimerHandle; // Timer handle for the explosion delay

    UFUNCTION()
    void Explode(); // Explode the projectile
};