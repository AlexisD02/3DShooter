// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MyCharacter.h"

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

    // Function declarations
    void CallJump();
    void CallStopJumping();
    void CallSprint();
    void CallStopSprinting();
    void CallStandToCrouch();
    void CallCrouchToStand();
    void CallForwards(float Value);
    void CallRight(float Value);
    void CallTurn(float Value);
    void CallLookUp(float Value);
    void CallProne();
    virtual void CallFire();
    void CallThrowGrenade();
    void CallOnReloadPressed();
    void CallAnimDance();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:
    AMyCharacter* ControlledCharacter;

    AMyGameModeBase* GameMode;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> CrosshairWidgetClass;

    UPROPERTY()
    UUserWidget* CrosshairWidget;

    void InitializeCrosshair();

    UPROPERTY(EditAnywhere)
    int AmmoCount = 10;

    // Reference to the pause menu widget class
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> PauseMenuWidgetClass;

    // The actual instance of the pause menu widget
    UPROPERTY()
    UUserWidget* PauseMenuWidget;

    // Function to toggle the pause menu
    void TogglePauseMenu();

    // Reference to the pause menu widget class
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> TimerWidgetClass;

    // The actual instance of the pause menu widget
    UPROPERTY()
    UUserWidget* TimerWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> GuideWidgetClass;

    UPROPERTY()
    UUserWidget* GuideWidget;

    // Function to Initialize and Display the Mini Map
    void InitializeMiniMap();

    void InitializeTimer();

    void InitializeGuide();

    void RemoveGuideFromViewport();

    UFUNCTION(BlueprintPure)
    int32 GetCurrentBullets();

    UFUNCTION(BlueprintPure)
    int32 GetTotalAmmunition();

    UFUNCTION(BlueprintPure)
    int32 GetGrenadesRemaining();

    UFUNCTION(BlueprintPure)
    float GetCurrentHealth();

    UFUNCTION(BlueprintPure)
    float GetMaxHealth();

    UFUNCTION(BlueprintPure)
    float GetCurrentStamina();

    UFUNCTION(BlueprintPure)
    float GetMaxStamina();

public:
    // Reference to the Mini Map Widget Class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> MiniMapWidgetClass;

    UPROPERTY()
    UUserWidget* MiniMapWidget;

    FTimerHandle TimerHandleRemoveWidget;

    float DelayInSeconds = 3.0f;

    void StartGame();

};
