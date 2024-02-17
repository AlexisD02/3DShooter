// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
AExplosiveProjectile::AExplosiveProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and setup components
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    RootComponent = ProjectileMesh;
    ProjectileMesh->SetSimulatePhysics(true);
    ProjectileMesh->SetNotifyRigidBodyCollision(true);

    //CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    //CollisionComponent->SetupAttachment(RootComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
    MovementComponent->SetUpdatedComponent(ProjectileMesh);
    MovementComponent->InitialSpeed = 1000.0f;
    MovementComponent->MaxSpeed = 1000.0f;

    BlastForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("BlastForceComponent"));
    BlastForceComponent->SetupAttachment(ProjectileMesh);
    BlastForceComponent->Radius = DamageRadius;
    BlastForceComponent->ImpulseStrength = ExplosionDamage * 15.0f;
}

// Called when the game starts or when spawned
void AExplosiveProjectile::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("BeginPlay called, setting explosion timer for %f seconds."), ExplosionDelay);
    // Set a timer to call the Explode function after a delay
    ProjectileMesh->OnComponentHit.AddDynamic(this, &AExplosiveProjectile::OnHit);

    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosiveProjectile::Explode, ExplosionDelay, false);
}

// Called every frame
void AExplosiveProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AExplosiveProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    HitEnemy = Cast<AEnemyCharacter>(OtherActor);
    // Check if the OtherActor is an enemy here. This condition is just an example.
    if (OtherActor && OtherActor != this && HitEnemy) {
        Explode();
    }
}

void AExplosiveProjectile::Explode()
{
    GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);

    UE_LOG(LogTemp, Warning, TEXT("Explode function called.")); 
    if (ExplosionEffect) {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
    }

    if (ExplosionSound) {
        UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
    }

    if (BlastForceComponent) {
        BlastForceComponent->FireImpulse();
    }

    UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), TArray<AActor*>(), this, nullptr, true);

    Destroy();
}
