// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyGameModeBase.generated.h"

/**
 * Game Mode class that defines basic game rules.
 */
UCLASS()
class CO2301_3DSHOOTER_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    AMyGameModeBase();

    // Functions accessible from Blueprints
    UFUNCTION(BlueprintCallable)
    void IncreaseScore();

    UFUNCTION(BlueprintCallable)
    void CheckGameStatus();

    UFUNCTION(BlueprintCallable)
    void EndGameDueToTime();

    UFUNCTION(BlueprintCallable)
    void EndGame(bool bPlayerWon);

    UFUNCTION(BlueprintCallable)
    FString GetFormattedCountdownTime();

    UFUNCTION(BlueprintPure)
    int32 CSToDisplay();

    UFUNCTION(BlueprintPure)
    int32 GetNumEnemiesLeft();

    UFUNCTION(BlueprintCallable)
    void ExtendTime();

    UFUNCTION()
    void TakeCountdown();

    UFUNCTION()
    void StartGame();

    UFUNCTION(BlueprintPure)
    FString GetCurrentCountdown();

    UFUNCTION()
    void DecreaseScore();

    UFUNCTION()
    void EnemiesLeft();

protected:
    virtual void BeginPlay() override;

private:
    // Properties
    UPROPERTY(EditDefaultsOnly)
    USoundBase* BackgroundMusic;

    UPROPERTY(VisibleAnywhere)
    FTimerHandle GameTimerHandle;

    UPROPERTY(VisibleAnywhere)
    FTimerHandle CountdownTimerHandle;

    UPROPERTY(VisibleAnywhere)
    FTimerHandle DecreaseScoreTimer;

    UPROPERTY(VisibleAnywhere)
    FTimerHandle EndGameTimer;

    UPROPERTY(EditDefaultsOnly)
    float GameDuration;

    UPROPERTY(EditDefaultsOnly)
    float GameEndTime = 0.0f;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* StartSound;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* CountdownSound;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* GoCountdownSound;

    UPROPERTY(VisibleAnywhere)
    int32 CurrentScoreToDisplay = 0;

    UPROPERTY(VisibleAnywhere)
    int32 CurrentScore = 0;

    UPROPERTY(VisibleAnywhere)
    int32 WinScore = 0;

    UPROPERTY(EditAnywhere)
    float ExtendTimeSec = 30.0f;

    UPROPERTY(VisibleAnywhere)
    int32 NumEnemiesLeft = 0;

    UPROPERTY(VisibleAnywhere)
    int32 CurrentCountdownValue = 6;

    UPROPERTY(VisibleAnywhere)
    float ScoreTimer = 100.0f;

    UPROPERTY(EditAnywhere)
    float DurationEnd = 2.0f;
};