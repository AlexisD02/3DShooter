// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleBarrel.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

ADestructibleBarrel::ADestructibleBarrel()
{
    PrimaryActorTick.bCanEverTick = false;

    BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
    RootComponent = BarrelMesh;

    ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionForce"));
    ExplosionForce->SetupAttachment(BarrelMesh);
    ExplosionForce->Radius = ExplosionRadius;
    ExplosionForce->ImpulseStrength = ExplosionStrength; // Adjust as needed

    // Initialize the FireEffect component and attach it to the BarrelMesh
    FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
    FireEffect->SetupAttachment(BarrelMesh);
}

void ADestructibleBarrel::BeginPlay()
{
    Super::BeginPlay();
}

float ADestructibleBarrel::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Health -= DamageAmount;
    if (Health <= 0.0f) {
        Explode();
    }
    return DamageAmount;
}

void ADestructibleBarrel::Explode()
{
    // Deactivate or destroy the fire effect if necessary
    FireEffect->DeactivateSystem();

    if (ExplosionForce) {
        ExplosionForce->FireImpulse();
    }

    if (ExplosionEffect) {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
    }

    if (BarrelExplosionSound) {
        UGameplayStatics::PlaySoundAtLocation(this, BarrelExplosionSound, GetActorLocation());
    }

    AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    TArray<AActor*> IgnoreEnemies;
    TArray<AActor*> EnemyCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemyCharacters);
    for (AActor* EnemyCharacter : EnemyCharacters) {
        IgnoreEnemies.Add(EnemyCharacter);
    }

    UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage / 3.0f, GetActorLocation(), ExplosionRadius * 2.0f / 3.0f, UDamageType::StaticClass(), IgnoreEnemies, this, nullptr, true);

    // Apply regular damage to other actors (non-main player)
    TArray<AActor*> IgnoreMainPlayer;
    if (PlayerCharacter) {
        IgnoreMainPlayer.Add(PlayerCharacter); // Ignore the main player
    }

    UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius * 3.0f / 4.0f, UDamageType::StaticClass(), IgnoreMainPlayer, this, nullptr, true);

    Destroy();
}
