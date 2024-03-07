// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveProjectile.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExplosiveProjectile::AExplosiveProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create and setup components
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    RootComponent = ProjectileMesh;
    ProjectileMesh->SetSimulatePhysics(true);
    ProjectileMesh->SetNotifyRigidBodyCollision(true);
    ProjectileMesh->BodyInstance.SetUseCCD(true); // Enable Continuous Collision Detection (CCD) for the collision component

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetupAttachment(RootComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
    MovementComponent->SetUpdatedComponent(ProjectileMesh);
    MovementComponent->InitialSpeed = 1200.0f;
    MovementComponent->MaxSpeed = 1200.0f;

    BlastForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("BlastForceComponent"));
    BlastForceComponent->SetupAttachment(ProjectileMesh);
    BlastForceComponent->Radius = DamageRadius;
    BlastForceComponent->ImpulseStrength = ExplosionDamage * 15.0f;
}

// Called when the game starts or when spawned
void AExplosiveProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Register hit event 
    ProjectileMesh->OnComponentHit.AddDynamic(this, &AExplosiveProjectile::OnHit); // Set a timer to call the Explode function after a delay
    // Register set timer for explosion
    GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosiveProjectile::Explode, ExplosionDelay, false);
}

void AExplosiveProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Check if the hit actor is an enemy, if so, explode
    AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(OtherActor);
    if (HitEnemy) Explode();
}

void AExplosiveProjectile::Explode()
{
    GetWorldTimerManager().ClearTimer(ExplosionTimerHandle); // Clear explosion timer

    // Spawn explosion effect
    if (ExplosionEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());

    // Play explosion sound
    if (ExplosionSound) UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

    // Apply fire impulse force
    if (BlastForceComponent) BlastForceComponent->FireImpulse();

    // Apply radial damage
    UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), TArray<AActor*>(), this, nullptr, true);

    Destroy(); // Destroy the projectile
}
