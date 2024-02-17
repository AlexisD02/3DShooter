// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Gun.h"
#include "ExplosiveProjectile.h"
#include "MiniMapCapture.h"
#include "EnemyCharacter.h"
#include "DestructibleBarrel.h"

#include "MyCharacter.generated.h"

UCLASS()
class CO2301_3DSHOOTER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AMyCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    //virtual void Tick(float DeltaTime) override;

    // Components for the third-person camera setup
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        USceneCaptureComponent2D* MiniMapCaptureComponent;

    // Spring arm for mini-map camera
    UPROPERTY(EditAnywhere)
        USpringArmComponent* MiniMapArm;

    UPROPERTY(EditAnywhere)
        TSubclassOf<AGun> WeaponGunClass;

    UPROPERTY(BlueprintReadOnly)
        AGun* WeaponGun;

    UPROPERTY(VisibleAnywhere)
        USceneComponent* ProjectileSpawnPoint;

    AEnemyCharacter* HitEnemy;

    ADestructibleBarrel* HitBarrel;

    UAnimInstance* AnimInstance;

    // Reload animation montage
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* ReloadAnimationMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* ShootAnimationMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* DanceAnimationMontage;

    // Function to attach the gun
    UFUNCTION(BlueprintCallable)
    void AttachGun();

    UFUNCTION()
    void Sprint();

    UFUNCTION()
    void StopSprinting();

    UFUNCTION()
    void Fire();

    UFUNCTION()
    void ThrowGrenade();

    // Reference to the grenade class to spawn
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AExplosiveProjectile> GrenadeClass;

    // Input axis functions
    void MoveForward(float Value);
    void MoveRight(float Value);

    // Method for handling crouch input
    UFUNCTION()
    void StandToCrouch();

    UFUNCTION()
    void CrouchToStand();

    void Turn(float Value);
    void LookUp(float Value);

    void ToJump();
    void ToStopJumping();

    void Prone();

    UFUNCTION()
    void AddAmmo();

    UFUNCTION()
    void AddHealth();

    UFUNCTION()
    void AddGrenade();

    UFUNCTION()
    void AnimDance();

    UFUNCTION()
    void EnableAllInput();
   
    // Crouch state variable exposed to Blueprint
    UPROPERTY(BlueprintReadWrite)
    bool bCrouchButtonDown;

    UPROPERTY(BlueprintReadWrite)
    bool bJumpButtonDown;

    UPROPERTY(BlueprintReadWrite)
    bool bIsProne;
    
    UPROPERTY(BlueprintReadWrite)
    bool bIsReloading;

    UPROPERTY(BlueprintReadWrite)
    bool bIsRunning;

    // Maximum number of bullets in a magazine
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxMagazineSize = 15;

    // Current number of bullets in the magazine
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 CurrentBulletsInMagazine;

    // Maximum number of magazines the character can carry
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxNumberOfMagazines = 2;

    // Total ammunition available for reloading
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 TotalAmmunition;
    
    // Function to handle reloading
    UFUNCTION()
    void Reload();

    // Handle for the reload timer
    FTimerHandle ReloadTimerHandle;

    FTimerHandle DanceTimerHandle;

    // Function to update the bullet count after reloading
    void UpdateBulletCountAfterReload();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GrenadesRemaining = 3;

    // Current and maximum health for the character
    UPROPERTY(VisibleAnywhere)
    float CurrentHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly)
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere)
    float HealthAmount = 20.0f;

    UPROPERTY(VisibleAnywhere)
    float CurrentStamina;

    UPROPERTY(EditDefaultsOnly)
    float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere)
    float StaminaDrainRate = 20.0f;

    UPROPERTY(EditAnywhere)
    float StaminaRecoveryRate = 5.0f;

    UFUNCTION()
    void DrainStamina();

    UFUNCTION()
    void RecoverStamina();

private:
    UPROPERTY(EditAnywhere)
    float SprintSpeedMultiplier;

    // Range of the cast used in the fire function. Default value set to 10000 units.
    UPROPERTY(EditAnywhere)
    float CastRange = 10000.0f;

    // Impulse force to be applied on the hit object. Default value set to 5000.
    UPROPERTY(EditAnywhere)
    float ImpulseForce = 5000.0f;

    UPROPERTY(EditAnywhere)
    float DamageAmount = 20.0f;

    // Function to handle damage and possibly die
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    FTimerHandle StaminaDrainTimerHandle;

    FTimerHandle StaminaRecoveryTimerHandle;
};
