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
#include "EnemyCharacter.h"

#include "ExplosiveProjectile.generated.h"

UCLASS()
class CO2301_3DSHOOTER_API AExplosiveProjectile : public AActor
{
	GENERATED_BODY()
	
public:
    // Sets default values for this actor's properties
    AExplosiveProjectile();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ProjectileMesh;

    //UPROPERTY(VisibleAnywhere)
    //USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(VisibleAnywhere)
    URadialForceComponent* BlastForceComponent;

    UPROPERTY(EditAnywhere)
    UParticleSystem* ExplosionEffect;

    // Variables
    UPROPERTY(EditAnywhere)
    float ExplosionDelay = 3.0f;

    UPROPERTY(EditAnywhere)
    float ExplosionDamage = 80.0f;

    UPROPERTY(EditAnywhere)
    float DamageRadius = 350.0f;

    UPROPERTY(EditAnywhere)
    USoundBase* ExplosionSound;

    // Timer handle for the explosion delay
    FTimerHandle ExplosionTimerHandle;

    // Functions
    void Explode();

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    AEnemyCharacter* HitEnemy;
};