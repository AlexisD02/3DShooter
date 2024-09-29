// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "MyGameModeBase.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
    PrimaryActorTick.bCanEverTick = false;  // Set this character to call Tick() to false

    AutoPossessPlayer = EAutoReceiveInput::Player0; // Auto Possess Player

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // Adjust this length as needed
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->SocketOffset = FVector(0, 35, 30); // Adjusted camera location
    CameraBoom->bEnableCameraLag = true; // Enable camera lag for smooth movement
    CameraBoom->CameraLagSpeed = 10.0f; // Adjust camera speed lag
    CameraBoom->CameraLagMaxDistance = 10.0f;

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom
    FollowCamera->SetRelativeLocation(FVector(0, 40, 20));
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Create a spawn point for projectile
    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
    ProjectileSpawnPoint->SetupAttachment(RootComponent);
    ProjectileSpawnPoint->SetRelativeLocation(FVector(40.0f, 0.0f, 60.0f));
    ProjectileSpawnPoint->SetRelativeRotation(FRotator(0.0f, 20.0f, 0.0f));

    // Initialize the Spring Arm Component for the Mini Map
    MiniMapArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapArm"));
    MiniMapArm->SetupAttachment(RootComponent);
    MiniMapArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f)); // Pointing downwards
    MiniMapArm->TargetArmLength = 600.0f; // Height above the character
    MiniMapArm->bDoCollisionTest = false; // Prevent the arm from adjusting its length

    // Initialize the Scene Capture Component for the Mini Map
    MiniMapCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MiniMapCapture"));
    MiniMapCaptureComponent->SetupAttachment(MiniMapArm);
    MiniMapCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
    MiniMapCaptureComponent->OrthoWidth = 4096.0f;
    MiniMapCaptureComponent->bCaptureEveryFrame = true;

    // Initialize ammunition counts
    CurrentBulletsInMagazine = MaxMagazineSize;
    TotalAmmunition = MaxMagazineSize * MaxNumberOfMagazines;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;
    bCrouchButtonDown = false;
    bJumpButtonDown = false;
    bIsProne = false;
    bIsReloading = false;
    bIsRunning = false;
    bMainPlayerDead = false;
    AnimInstance = GetMesh()->GetAnimInstance();
    CurrentStamina = MaxStamina;

    AttachGun();
}

void AMyCharacter::AttachGun()
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

void AMyCharacter::StandToCrouch()
{
    // Transition from standing to crouching if possible and check if not prone
    if (CanCrouch() && !bIsProne) { 
        Crouch();
        bCrouchButtonDown = true;
    }
}

void AMyCharacter::CrouchToStand()
{
    // Transition from crouching to standing and check if not prone
    if (!bIsProne) { 
        UnCrouch();
        bCrouchButtonDown = false;
    }
}

void AMyCharacter::Sprint()
{
    // Start sprinting if not already sprinting and sufficient stamina
    if (!bIsRunning && CurrentStamina > 0) {
        bIsRunning = true;
        GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
        GetWorldTimerManager().ClearTimer(StaminaRecoveryTimerHandle);
        GetWorldTimerManager().SetTimer(StaminaDrainTimerHandle, this, &AMyCharacter::DrainStamina, 0.2f, true);
    }
}

void AMyCharacter::StopSprinting()
{
    // Stop sprinting
    if (bIsRunning) { 
        bIsRunning = false;
        GetCharacterMovement()->MaxWalkSpeed = 600.0f;
        GetWorldTimerManager().ClearTimer(StaminaDrainTimerHandle);
        GetWorldTimerManager().SetTimer(StaminaRecoveryTimerHandle, this, &AMyCharacter::RecoverStamina, 0.2f, true);
    }
}

void AMyCharacter::DrainStamina()
{
    // Drain stamina while sprinting
    CurrentStamina -= StaminaDrainRate * 0.1f;
    if (CurrentStamina <= 0) {
        CurrentStamina = 0;
        StopSprinting();
    }
}

void AMyCharacter::RecoverStamina()
{
    // Recover stamina over time
    CurrentStamina += StaminaRecoveryRate * 0.1f;
    if (CurrentStamina >= MaxStamina) {
        CurrentStamina = MaxStamina;
        GetWorldTimerManager().ClearTimer(StaminaRecoveryTimerHandle);
    }
}

void AMyCharacter::MoveForward(float Value)
{
    // Move character forward/backward and check if not prone
    if (!bIsProne && Value != 0.0f) {
        FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AMyCharacter::MoveRight(float Value)
{
    // Move character right/left and check if not prone
    if (!bIsProne && Value != 0.0f) {
        FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AMyCharacter::Turn(float Value)
{
    // Turn character horizontally
    AddControllerYawInput(Value);
}

void AMyCharacter::LookUp(float Value)
{
    // Look up/down
    AddControllerPitchInput(Value);
}

void AMyCharacter::ToJump()
{
    if (CanJump()) { // Assuming CanJump() is a method that checks if the character can jump
        Jump();
        bJumpButtonDown = true;
    }
}

void AMyCharacter::ToStopJumping()
{
    StopJumping();
    bJumpButtonDown = false;
}

void AMyCharacter::Prone()
{
    if (!bCrouchButtonDown) {
        if (!bIsProne) {
            Crouch();
            bIsProne = true; // Transition to prone
        }
        else {
            UnCrouch();
            bIsProne = false; // Transition out of prone
        }
    }
}

void AMyCharacter::Fire()
{
    if (!bIsReloading && CurrentBulletsInMagazine > 0) {
        if (!bIsRunning) {
            CurrentBulletsInMagazine--;

            if (WeaponGun && WeaponGunClass) WeaponGun->Fire(); // Call the Fire function

            if (AnimInstance && ShootAnimationMontage) AnimInstance->Montage_Play(ShootAnimationMontage);

            AController* ControllerRef = GetController();
            if (!ControllerRef) return; // Early return if no controller

            FVector CameraLocation;
            FRotator CameraRotation;
            ControllerRef->GetPlayerViewPoint(CameraLocation, CameraRotation);

            // Calculate the end point of the ray
            FVector End = CameraLocation + (CameraRotation.Vector() * CastRange);

            FHitResult HitResult;

            // Perform the raycast
            if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_Visibility)) {
                // Check if the hit actor is an enemy character
                AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(HitResult.GetActor());
                if (HitEnemy) {
                    // Apply damage to an enemy character
                    UGameplayStatics::ApplyDamage(HitEnemy, DamageAmount, ControllerRef, this, UDamageType::StaticClass());
                }

                // Check if the hit actor is the destructible barrel
                ADestructibleBarrel* HitBarrel = Cast<ADestructibleBarrel>(HitResult.GetActor());
                if (HitBarrel) {
                    // Apply damage to the barrel
                    UGameplayStatics::ApplyDamage(HitBarrel, DamageAmount, ControllerRef, this, UDamageType::StaticClass());
                }
            }
        }
    }
    else if (!bIsReloading) {
        if (WeaponGun && WeaponGunClass) WeaponGun->EmptyMug();
    }
}

void AMyCharacter::Reload()
{
    if ((TotalAmmunition > 0 && CurrentBulletsInMagazine < MaxMagazineSize) && !bIsReloading) {
        bIsReloading = true;
        if (AnimInstance && ReloadAnimationMontage) {
            AnimInstance->Montage_Play(ReloadAnimationMontage);
            float AnimationDuration = ReloadAnimationMontage->GetPlayLength();

            // Set a timer to update the bullet count after the animation plays
            GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AMyCharacter::UpdateBulletCountAfterReload, AnimationDuration, false);
        }
    }
}

void AMyCharacter::UpdateBulletCountAfterReload()
{
    // Update bullet count after reloading
    bIsReloading = false;
    int32 BulletsNeeded = MaxMagazineSize - CurrentBulletsInMagazine;
    int32 BulletsToReload = FMath::Min(BulletsNeeded, TotalAmmunition);

    CurrentBulletsInMagazine += BulletsToReload;
    TotalAmmunition -= BulletsToReload;
}

void AMyCharacter::AddAmmo()
{
    TotalAmmunition += MaxMagazineSize * MaxNumberOfMagazines; // Add ammunition
}

void AMyCharacter::AddHealth()
{
    CurrentHealth = FMath::Min(CurrentHealth + HealthAmount, MaxHealth); // Add health, ensuring it doesn't exceed the maximum health
}

void AMyCharacter::AddGrenade()
{
    GrenadesRemaining++; // Increment the number of grenades remaining
}

void AMyCharacter::AnimDance()
{
    AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    PlayerController->DisableInput(PlayerController); // Disable all main player input

    if (WeaponGun) WeaponGun->SetActorHiddenInGame(true); // Set the visibility of the gun to true

    if (AnimInstance && DanceAnimationMontage) {
        AnimInstance->Montage_Play(DanceAnimationMontage);
        float DanceAnimationDuration = DanceAnimationMontage->GetPlayLength();

        // Set a timer to update the bullet count after the animation plays
        GetWorldTimerManager().SetTimer(DanceTimerHandle, this, &AMyCharacter::EnableAllInput, DanceAnimationDuration, false);
    }
}

void AMyCharacter::EnableAllInput()
{
    AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    PlayerController->EnableInput(PlayerController); // Enable all main player input
    if (WeaponGun) WeaponGun->SetActorHiddenInGame(false); // Set the visibility of the gun to true
}

void AMyCharacter::ThrowGrenade()
{
    if ((GrenadeClass && ProjectileSpawnPoint) && GrenadesRemaining > 0) {
        // Use the ProjectileSpawnPoint's position and rotation for spawning the grenade
        FVector Location = ProjectileSpawnPoint->GetComponentLocation();
        FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

        // Spawn the grenade
        Grenade = GetWorld()->SpawnActor<AExplosiveProjectile>(GrenadeClass, Location, Rotation);
        if (Grenade) { 
            GrenadesRemaining--;
            Grenade->SetOwner(this); // Set the owner of the grenade to this character
        }
    }
}

float AMyCharacter::TakeDamage(float DamageAmountChar, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // Call the base class - this is not required, but can be useful if it does something
    float DamageApplied = Super::TakeDamage(DamageAmountChar, DamageEvent, EventInstigator, DamageCauser);

    // Apply the damage to the character's health
    CurrentHealth -= DamageApplied;

    // Check if the character is out of health and should die
    if (CurrentHealth <= 0.0f) {
        bMainPlayerDead = true;
        BP_MainPlayerDead();
        AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
        if (PlayerController) PlayerController->DisableInput(PlayerController);
        if (DeathSound) UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
        if (PlayerController) PlayerController->RemoveAllWidgets();
        GetWorldTimerManager().SetTimer(GameOverTimer, this, &AMyCharacter::DisplayGameOverScreen, 2.0f, false);
    }

    // Return the actual damage taken, which might be different from DamageAmount due to modifiers or armor
    return DamageApplied;
}

void AMyCharacter::DisplayGameOverScreen()
{
    // Retrieve the game mode and end the game with player loss
    AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    GameMode->EndGame(false); // Player lost
}