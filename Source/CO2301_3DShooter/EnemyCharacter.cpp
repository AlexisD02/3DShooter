// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MyCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
    // Set this character to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    AttachGun();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AEnemyCharacter::InitializeEnemyCountForGameMode(UWorld* World)
{
    int32 EnemyCount = 0;
    for (TActorIterator<AEnemyCharacter> It(World); It; ++It)
    {
        EnemyCount++;
    }

    AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(World->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->SetWinScore(EnemyCount);
    }
}

void AEnemyCharacter::AttachGun()
{
    if (WeaponGunClass) {
        // Get the transform of the socket to which we want to attach the gun
        const FTransform GunSocketTransform = GetMesh()->GetSocketTransform(TEXT("GunSocket"), ERelativeTransformSpace::RTS_World);

        // Spawn the gun actor at the socket's transform
        WeaponGun = GetWorld()->SpawnActor<AGun>(WeaponGunClass, GunSocketTransform);
        if (!WeaponGun) {
            UE_LOG(LogTemp, Warning, TEXT("WeaponGun is null! Check the WeaponGunClass and socket."));
            return;
        }

        // Attach the gun to the socket named "GunSocket" on the character's mesh
        WeaponGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GunSocket"));
    }
}

void AEnemyCharacter::DestroyGun()
{
    if (WeaponGun) {
        WeaponGun->Destroy();
        WeaponGun = nullptr; // Nullify the pointer after destruction to avoid dangling references.
    }
}

void AEnemyCharacter::Fire()
{
    if (WeaponGun) {
        WeaponGun->Fire(); // Assuming enemies use a similar firing mechanism
    }

    FVector Start = GetActorLocation();
    FVector End = Start + (GetActorForwardVector() * 1000.0f); // Adjust range as needed
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

    DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 1.0f, 0, 1.0f);

    if (bHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
        AMyCharacter* HitCharacter = Cast<AMyCharacter>(HitResult.GetActor());
        if (HitCharacter)
        {
            float DamageDealt = UGameplayStatics::ApplyDamage(HitCharacter, 10.0f, GetController(), this, UDamageType::StaticClass());
            UE_LOG(LogTemp, Warning, TEXT("Damage Dealt: %f"), DamageDealt);
        }
    }
}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // Log the hit for now, you can replace this with your own logic later
    UE_LOG(LogTemp, Warning, TEXT("Bullet hit an enemy"));

    // Decrement the Health by the damage amount
    Health -= DamageApplied;

    if (Health <= 0.0f) {
        // Safely obtain a reference to the game mode and call IncreaseScore
        MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
        if (MyGameMode != nullptr) { // Ensure the game mode instance is valid
            MyGameMode->IncreaseScore();
            MyGameMode->EnemiesLeft();
        }
        Destroy();
        DestroyGun();
    }

    return DamageApplied;
}