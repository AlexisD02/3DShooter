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
	ADestructibleBarrel(); // Sets default values for this actor's properties

protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned

public:	
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    UPROPERTY(EditDefaultsOnly)
    float ExplosionDamage = 80.0f;

    UPROPERTY(EditDefaultsOnly)
    float ExplosionRadius = 800.0f;

    UPROPERTY(EditDefaultsOnly)
    float ExplosionStrength = 2000.0f;

    UPROPERTY(EditAnywhere)
    float Health = 40.0f; // Health of the barrel

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BarrelMesh;

    UPROPERTY(VisibleAnywhere)
    URadialForceComponent* ExplosionForce;

    UPROPERTY(VisibleAnywhere)
    UParticleSystemComponent* FireEffect;

    UPROPERTY(EditAnywhere)
    UParticleSystem* ExplosionEffect; // Explosion effect

    UPROPERTY(EditDefaultsOnly)
    USoundBase* BarrelExplosionSound;

    UFUNCTION()
    void Explode(); // Helper function to handle the explosion
};