// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "MyGameModeBase.h"

AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    AttachGun(); // Attach the gun to the character

    bDamageReceived = false;
    bIsDead = false;
}

void AEnemyCharacter::AttachGun()
{
    if (WeaponGunClass) { // Check if the weapon class is valid
        // Get the transform of the socket to which we want to attach the gun
        const FTransform GunSocketTransform = GetMesh()->GetSocketTransform(TEXT("GunSocket"), ERelativeTransformSpace::RTS_World);

        // Spawn the gun actor at the socket's transform
        WeaponGun = GetWorld()->SpawnActor<AGun>(WeaponGunClass, GunSocketTransform);
        if (!WeaponGun) { // Check if gun actor was spawned successfully
            UE_LOG(LogTemp, Warning, TEXT("WeaponGun is null! Check the WeaponGunClass and socket."));
            return;
        }

        // Attach the gun to the socket named "GunSocket" on the character's mesh
        WeaponGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GunSocket"));
    }
}

void AEnemyCharacter::DestroyGun()
{
    if (WeaponGun) { // Check if the gun exists before destroying
        WeaponGun->Destroy();
        WeaponGun = nullptr; // Nullify the pointer after destruction to avoid dangling references.
    }
}

void AEnemyCharacter::Fire()
{
    if (!bIsDead) { // Check if the enemy is alive
        if (WeaponGun) WeaponGun->Fire(); // Fire the gun

        // Calculate the start and end locations for the line trace
        FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 20.0f);
        FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(); // Get player's location
        FVector DirectionToPlayer = (PlayerLocation - Start).GetSafeNormal(); // Direction vector from enemy to player
        FVector End = Start + (DirectionToPlayer * 1000.0f);

        // Adjust the height based on the player's height relative to the enemy
        float PlayerHeightDifference = PlayerLocation.Z - Start.Z;
        End.Z += PlayerHeightDifference;

        FHitResult HitResult; // Perform line trace to check for hit on player
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this); // Ignore the enemy character

        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params)) { // Check if the line trace hits anything
            AMyCharacter* HitCharacter = Cast<AMyCharacter>(HitResult.GetActor());
            // Apply damage to the player
            if (HitCharacter) UGameplayStatics::ApplyDamage(HitCharacter, 10.0f, GetController(), this, UDamageType::StaticClass());
        }
    }
}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); // Call parent class function to handle damage

    // Decrement the Health by the damage amount
    Health -= DamageApplied;

    if (Health <= 0.0f) {
        // Safely obtain a reference to the game mode and call IncreaseScore
        AMyGameModeBase* MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
        if (MyGameMode != nullptr) { // Ensure the game mode instance is valid
            MyGameMode->IncreaseScore();
            MyGameMode->EnemiesLeft();
        }
        EnemyIsDead();
    }
    else {
        bDamageReceived = true;
    }

    return DamageApplied;
}

void AEnemyCharacter::EnemyIsDead()
{
    bIsDead = true;
    BP_EnemyIsDead();
    GetWorldTimerManager().SetTimer(DestroyEnemyTimer, this, &AEnemyCharacter::DestroyEnemy, DestroyEnemyTime, false);
}

void AEnemyCharacter::DestroyEnemy()
{
    Destroy();
    DestroyGun();
}