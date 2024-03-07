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

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
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

    UFUNCTION()
    void MoveForward(float Value);

    UFUNCTION()
    void MoveRight(float Value);

    UFUNCTION()
    void StandToCrouch();

    UFUNCTION()
    void CrouchToStand();

    UFUNCTION()
    void Turn(float Value);

    UFUNCTION()
    void LookUp(float Value);

    UFUNCTION()
    void ToJump();

    UFUNCTION()
    void ToStopJumping();

    UFUNCTION()
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
    
    UFUNCTION()
    void Reload();

    UFUNCTION()
    void UpdateBulletCountAfterReload();

    UFUNCTION()
    void DrainStamina();

    UFUNCTION()
    void RecoverStamina();

    UFUNCTION()
    void DisplayGameOverScreen();

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bMainPlayerDead;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxMagazineSize = 15; // Maximum number of bullets in a magazine

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 CurrentBulletsInMagazine; // Current number of bullets in the magazine

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxNumberOfMagazines = 2; // Maximum number of magazines the character can carry

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 TotalAmmunition; // Total ammunition available for reloading

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GrenadesRemaining = 3;

    UPROPERTY(VisibleAnywhere)
    float CurrentHealth = 100.0f; // Current and maximum health for the character

    UPROPERTY(EditAnywhere)
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere)
    float HealthAmount = 20.0f;

    UPROPERTY(VisibleAnywhere)
    float CurrentStamina;

    UPROPERTY(EditAnywhere)
    float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere)
    float StaminaDrainRate = 20.0f;

    UPROPERTY(EditAnywhere)
    float StaminaRecoveryRate = 5.0f;

    UPROPERTY(EditAnywhere)
    float SprintSpeedMultiplier;

    UPROPERTY(EditAnywhere)
    float CastRange = 10000.0f;

    UPROPERTY(EditAnywhere)
    float ImpulseForce = 5000.0f;

    UPROPERTY(EditAnywhere)
    float DamageAmount = 20.0f;

    UFUNCTION(BlueprintImplementableEvent)
    void BP_MainPlayerDead();

    // Components for the third-person camera setup
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USceneCaptureComponent2D* MiniMapCaptureComponent;

    UAnimInstance* AnimInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* ReloadAnimationMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* ShootAnimationMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* DanceAnimationMontage;

private:
    UPROPERTY(EditAnywhere)
    USpringArmComponent* MiniMapArm;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* ProjectileSpawnPoint;

    UPROPERTY(EditAnywhere)
    USoundBase* DeathSound;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AGun> WeaponGunClass;

    AGun* WeaponGun;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AExplosiveProjectile> GrenadeClass;

    AExplosiveProjectile* Grenade;

    FTimerHandle ReloadTimerHandle;

    FTimerHandle DanceTimerHandle;

    FTimerHandle StaminaDrainTimerHandle;

    FTimerHandle StaminaRecoveryTimerHandle;

    FTimerHandle GameOverTimer;
};
