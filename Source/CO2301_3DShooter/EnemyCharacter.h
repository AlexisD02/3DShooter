// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Gun.h"

#include "EnemyCharacter.generated.h"

UCLASS()
class CO2301_3DSHOOTER_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
	float Health = 100.0f;

	// Function to attach the gun
	UFUNCTION(BlueprintCallable)
	void AttachGun();

	UFUNCTION()
	void Fire();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> WeaponGunClass;

	UPROPERTY(BlueprintReadOnly)
	AGun* WeaponGun;

	UFUNCTION()
	void DestroyGun();

	UPROPERTY(BlueprintReadOnly)
	bool bDamageReceived;

	UFUNCTION()
	void DestroyEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDead;

	UFUNCTION()
	void EnemyIsDead();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_EnemyIsDead();

private:
	FTimerHandle DestroyEnemyTimer;

	UPROPERTY(EditAnywhere)
	float DestroyEnemyTime = 3.0f;
};
