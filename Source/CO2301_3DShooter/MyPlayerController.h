// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MyCharacter.h"
#include "MyGameModeBase.h"

#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CO2301_3DSHOOTER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AMyPlayerController();

    virtual void SetupInputComponent() override;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void CallJump();

    UFUNCTION()
    void CallStopJumping();

    UFUNCTION()
    void CallSprint();

    UFUNCTION()
    void CallStopSprinting();

    UFUNCTION()
    void CallStandToCrouch();

    UFUNCTION()
    void CallCrouchToStand();

    UFUNCTION()
    void CallForwards(float Value);

    UFUNCTION()
    void CallRight(float Value);

    UFUNCTION()
    void CallTurn(float Value);

    UFUNCTION()
    void CallLookUp(float Value);

    UFUNCTION()
    void CallProne();

    UFUNCTION()
    void CallFire();

    UFUNCTION()
    void CallThrowGrenade();

    UFUNCTION()
    void CallOnReloadPressed();

    UFUNCTION()
    void CallAnimDance();

    UFUNCTION()
    void InitializeCrosshair();

    UFUNCTION()
    void TogglePauseMenu();

    UFUNCTION()
    void InitializeMiniMap();

    UFUNCTION()
    void InitializeTimer();

    UFUNCTION()
    void InitializeGuide();

    UFUNCTION()
    void RemoveGuideFromViewport();

    AMyCharacter* ControlledCharacter;

    AMyGameModeBase* GameMode;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> CrosshairWidgetClass;

    UPROPERTY()
    UUserWidget* CrosshairWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> PauseMenuWidgetClass;

    UPROPERTY()
    UUserWidget* PauseMenuWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> TimerWidgetClass;

    UPROPERTY()
    UUserWidget* TimerWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> GuideWidgetClass;

    UPROPERTY()
    UUserWidget* GuideWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> MiniMapWidgetClass;

    UPROPERTY()
    UUserWidget* MiniMapWidget;

    FTimerHandle TimerHandleRemoveWidget;

public:
    UFUNCTION()
    void StartGame();

    UFUNCTION(BlueprintPure)
    float GetMaxStamina();

    UFUNCTION(BlueprintPure)
    float GetCurrentHealth();

    UFUNCTION(BlueprintPure)
    float GetMaxHealth();

    UFUNCTION(BlueprintPure)
    float GetCurrentStamina();

    UFUNCTION(BlueprintPure)
    int32 GetCurrentBullets();

    UFUNCTION(BlueprintPure)
    int32 GetTotalAmmunition();

    UFUNCTION(BlueprintPure)
    int32 GetGrenadesRemaining();
};
