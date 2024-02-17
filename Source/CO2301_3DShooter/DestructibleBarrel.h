// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/DamageType.h"
#include "UObject/ObjectMacros.h"
#include "Particles/ParticleSystemComponent.h"

#include "DestructibleBarrel.generated.h"

UCLASS()
class CO2301_3DSHOOTER_API ADestructibleBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    // Override the TakeDamage function to interact with Unreal's damage system
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    // Components
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BarrelMesh;

    UPROPERTY(VisibleAnywhere)
    URadialForceComponent* ExplosionForce;

    // Explosion effect
    UPROPERTY(EditAnywhere)
    UParticleSystem* ExplosionEffect;

    // Damage settings
    UPROPERTY(EditDefaultsOnly)
    float ExplosionDamage = 80.0f;

    UPROPERTY(EditDefaultsOnly)
    float ExplosionRadius = 800.0f;

    UPROPERTY(EditDefaultsOnly)
    float ExplosionStrength = 2000.0f;

    UPROPERTY(VisibleAnywhere)
    UParticleSystemComponent* FireEffect;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* BarrelExplosionSound;

private:
    // Health of the barrel
    UPROPERTY(EditAnywhere)
    float Health = 40.0f;

    // Helper function to handle the explosion
    void Explode();
};