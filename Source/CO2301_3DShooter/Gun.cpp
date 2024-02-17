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
    PrimaryActorTick.bCanEverTick = true;

    // Create a gun mesh component
    WeaponGunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
    RootComponent = WeaponGunMesh;

    // Initialize sound (ensure you have these assets in your project)
    FireSound = nullptr; // Assign this in the editor or dynamically

    // Initialize muzzle flash (optional)
    MuzzleFlash = nullptr; // Assign this in the editor or dynamically
}

void AGun::BeginPlay()
{
    Super::BeginPlay();
}

void AGun::Fire()
{
    // Play fire sound if available
    if (FireSound != nullptr) {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    // Play muzzle flash if available
    if (MuzzleFlash != nullptr) {
        // Spawn the emitter attached to the weapon's gun mesh
        UParticleSystemComponent* MuzzleFlashComponent = UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponGunMesh, TEXT("Muzzle"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale);

        if (MuzzleFlashComponent != nullptr) {
            // Adjust the scale of the muzzle flash effect
            FVector Scale = FVector(0.1f); // Example scale, adjust as needed
            MuzzleFlashComponent->SetWorldScale3D(Scale);
        }
    }

}

void AGun::EmptyMug()
{
    // Play fire sound if available
    if (EmptyMugSound != nullptr) {
        UGameplayStatics::PlaySoundAtLocation(this, EmptyMugSound, GetActorLocation());
    }
}