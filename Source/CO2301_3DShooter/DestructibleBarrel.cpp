// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleBarrel.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

ADestructibleBarrel::ADestructibleBarrel()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create and set up the barrel mesh
    BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
    RootComponent = BarrelMesh;

    // Create and set up the explosion force component
    ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionForce"));
    ExplosionForce->SetupAttachment(BarrelMesh);
    ExplosionForce->Radius = ExplosionRadius;
    ExplosionForce->ImpulseStrength = ExplosionStrength; // Adjust as needed

    // Create and set up the fire effect component
    FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
    FireEffect->SetupAttachment(BarrelMesh);
}

void ADestructibleBarrel::BeginPlay()
{
    Super::BeginPlay();
}

float ADestructibleBarrel::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Health -= DamageAmount; // Reduce health based on the damage received
    if (Health <= 0.0f) Explode(); // Explode if health drops to zero or below
    return DamageAmount;
}

void ADestructibleBarrel::Explode()
{
    FireEffect->DeactivateSystem(); // Deactivate or destroy the fire effect if necessary

    if (ExplosionForce) ExplosionForce->FireImpulse(); // Fire the explosion force

    // Spawn explosion effect at the barrel's location
    if (ExplosionEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());

    // Play barrel explosion sound at the barrel's location
    if (BarrelExplosionSound) UGameplayStatics::PlaySoundAtLocation(this, BarrelExplosionSound, GetActorLocation());

    // Get the main player character
    AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // Ignore enemy characters for radial damage
    TArray<AActor*> IgnoreEnemies;
    TArray<AActor*> EnemyCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemyCharacters);
    for (AActor* EnemyCharacter : EnemyCharacters) {
        IgnoreEnemies.Add(EnemyCharacter);
    }

    // Apply radial damage to enemies
    UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage / 3.0f, GetActorLocation(), ExplosionRadius * 2.0f / 3.0f, UDamageType::StaticClass(), IgnoreEnemies, this, nullptr, true);

    // Ignore main player for radial damage
    TArray<AActor*> IgnoreMainPlayer;
    if (PlayerCharacter) {
        IgnoreMainPlayer.Add(PlayerCharacter); // Ignore the main player
    }

    // Apply regular damage to other actors (non-main player)
    UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius * 3.0f / 4.0f, UDamageType::StaticClass(), IgnoreMainPlayer, this, nullptr, true);

    Destroy(); // Destroy the barrel after explosion
}
