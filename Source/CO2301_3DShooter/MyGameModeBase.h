// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h" // Include this header
#include "EnemyAIController.h"
#include "MyGameModeBase.generated.h"

/**
 * Game Mode class that defines basic game rules.
 */
UCLASS()
class CO2301_3DSHOOTER_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    // Constructor
    AMyGameModeBase();

    // Function to increase the game score
    UFUNCTION(BlueprintCallable)
    void IncreaseScore();

    // Function to update the game challenge level
    UFUNCTION(BlueprintCallable)
    void IncreaseChallenge();

    UFUNCTION(BlueprintCallable)
    void CheckGameStatus();

    // Function to start the game timer
    UFUNCTION(BlueprintCallable)
    void GameTimer();

    UFUNCTION()
    void UpdateGameTimer();

    UFUNCTION(BlueprintCallable)
    void EndGameDueToTime();

    UFUNCTION(BlueprintCallable)
    void MainCharacterKilled();

    UFUNCTION(BlueprintCallable)
    void EndGame(bool bPlayerWon);

    // Function to get the formatted countdown time (MM:SS)
    UFUNCTION(BlueprintCallable)
    FString GetFormattedCountdownTime();

    // Current score of the game
    UPROPERTY(VisibleAnywhere)
    int CurrentScore = 0;

    // Method to set the winning score
    void SetWinScore(int32 NewWinScore);

    // Score needed to win the game
    UPROPERTY(VisibleAnywhere)
    int32 WinScore = 0;

    UFUNCTION()
    void ExtendTime();

    UPROPERTY(EditAnywhere)
    float ExtendTimeSec = 30.0f;

    UFUNCTION()
    void EnemiesLeft();

    UPROPERTY(BlueprintReadWrite)
    int32 NumEnemiesLeft;

    UFUNCTION(BlueprintPure)
    int32 GetNumEnemiesLeft();

    void DisableAllCharacterInput();

    void TakeCountdown();
    
    void StartGame();

    UPROPERTY(BlueprintReadWrite)
    int32 ValueToDisplay;

    UPROPERTY(BlueprintReadWrite)
    int32 CurrentCountdownValue = 6;

    UFUNCTION(BlueprintPure)
    FString GetCurrentCountdown();
        
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Background music asset reference
    UPROPERTY(EditDefaultsOnly)
    USoundBase* BackgroundMusic;

private:
    // Current level of challenge/difficulty in the game
    UPROPERTY(VisibleAnywhere)
    int ChallengeLevel;

    // Handle for efficient management of the game timer
    FTimerHandle GameTimerHandle;

    FTimerHandle CountdownTimerHandle;

    // Game duration in seconds
    UPROPERTY(EditDefaultsOnly)
    float GameDuration;

    // Timestamp for when the game will end
    float GameEndTime;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* StartSound;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* CountdownSound;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* GoCountdownSound;
};