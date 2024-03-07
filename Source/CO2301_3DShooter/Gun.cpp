// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


AGun::AGun()
{
    // Set this actor to call Tick() if required
    PrimaryActorTick.bCanEverTick = false;

    // Create a gun mesh component
    WeaponGunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
    RootComponent = WeaponGunMesh;
}

void AGun::Fire()
{
    // Play fire sound if available
    if (FireSound) UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

    // Play muzzle flash if available
    if (MuzzleFlash) {
        // Spawn the emitter attached to the weapon's gun mesh
        UParticleSystemComponent* MuzzleFlashComponent = UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponGunMesh, TEXT("Muzzle"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale);

        if (MuzzleFlashComponent) {
            MuzzleFlashComponent->SetWorldScale3D(FVector(0.1f)); // Adjust the scale of the muzzle flash effect
        }
    }
}

void AGun::EmptyMug()
{
    // Play fire sound if available
    if (EmptyMugSound) UGameplayStatics::PlaySoundAtLocation(this, EmptyMugSound, GetActorLocation());
}