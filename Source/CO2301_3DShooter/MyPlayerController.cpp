// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "GameFramework/Character.h"

AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    ControlledCharacter = Cast<AMyCharacter>(GetPawn());

    PauseMenuWidget = nullptr;

    InitializeGuide();
}

void AMyPlayerController::StartGame() 
{
    RemoveGuideFromViewport();
    InitializeCrosshair();
    InitializeTimer();
    InitializeMiniMap();
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    check(InputComponent);

    // Bind jump events
    InputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayerController::CallJump);
    InputComponent->BindAction("Jump", IE_Released, this, &AMyPlayerController::CallStopJumping);

    // Bind sprint event
    InputComponent->BindAction("Sprint", IE_Pressed, this, &AMyPlayerController::CallSprint);
    InputComponent->BindAction("Sprint", IE_Released, this, &AMyPlayerController::CallStopSprinting);

    //// Bind crouch toggle
    InputComponent->BindAction("Crouch", IE_Pressed, this, &AMyPlayerController::CallStandToCrouch);
    InputComponent->BindAction("Crouch", IE_Released, this, &AMyPlayerController::CallCrouchToStand);

    // Bind fire event
    InputComponent->BindAction("Fire", IE_Pressed, this, &AMyPlayerController::CallFire);

    // Bind the ToggleProne action.
    InputComponent->BindAction("Prone", IE_Pressed, this, &AMyPlayerController::CallProne);

    // Bind movement axes
    InputComponent->BindAxis("MoveForward", this, &AMyPlayerController::CallForwards);
    InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::CallRight);

    // Bind camera axes
    InputComponent->BindAxis("Turn", this, &AMyPlayerController::CallTurn);
    InputComponent->BindAxis("LookUp", this, &AMyPlayerController::CallLookUp);

    InputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AMyPlayerController::CallThrowGrenade);

    InputComponent->BindAction("Reload", IE_Pressed, this, &AMyPlayerController::CallOnReloadPressed);

    InputComponent->BindAction("AnimDance", IE_Pressed, this, &AMyPlayerController::CallAnimDance);

    // Bind the TogglePauseMenu action
    InputComponent->BindAction("TogglePause", IE_Pressed, this, &AMyPlayerController::TogglePauseMenu);
}

void AMyPlayerController::InitializeGuide()
{
    if (GuideWidgetClass) {
        GuideWidget = CreateWidget(this, GuideWidgetClass);
        if (GuideWidget) GuideWidget->AddToViewport();
    }
}

void AMyPlayerController::RemoveAllWidgets()
{
    if (CrosshairWidget) CrosshairWidget->RemoveFromParent();
    if (TimerWidget) TimerWidget->RemoveFromParent();
    if (MiniMapWidget) MiniMapWidget->RemoveFromParent();
}

void AMyPlayerController::RemoveGuideFromViewport()
{
    // Remove the widget from the viewport
    if (GuideWidget) GuideWidget->RemoveFromParent();
}

void AMyPlayerController::InitializeCrosshair()
{
    // Check if the CrosshairWidgetClass is valid before trying to create it
    if (CrosshairWidgetClass) {
        CrosshairWidget = CreateWidget(this, CrosshairWidgetClass);
        if (CrosshairWidget) CrosshairWidget->AddToViewport();
    }
}

void AMyPlayerController::InitializeTimer()
{
    if (TimerWidgetClass) {
        TimerWidget = CreateWidget(this, TimerWidgetClass);
        if (TimerWidget) TimerWidget->AddToViewport();
    }
}

void AMyPlayerController::InitializeMiniMap()
{
    if (MiniMapWidgetClass) {
        MiniMapWidget = CreateWidget(this, MiniMapWidgetClass);
        if (MiniMapWidget) {
            MiniMapWidget->AddToViewport();
        }
    }
}

void AMyPlayerController::TogglePauseMenu()
{
    if (!PauseMenuWidget && PauseMenuWidgetClass) { // Create the pause menu widget if it doesn't exist
        PauseMenuWidget = CreateWidget(this, PauseMenuWidgetClass);
    }

    if (PauseMenuWidget) { // Add the widget to the viewport and pause the game
        PauseMenuWidget->AddToViewport();
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        SetPause(true);
    }
}

void AMyPlayerController::DisplayWinScreen()
{
    if (!WinScreenWidget && WinScreenWidgetClass) { // Create the pause menu widget if it doesn't exist
        WinScreenWidget = CreateWidget(this, WinScreenWidgetClass);
    }

    if (WinScreenWidget) { // Add the widget to the viewport and pause the game
        WinScreenWidget->AddToViewport();
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        SetPause(true);
    }
}

void AMyPlayerController::DisplayLoseScreen()
{
    if (!LoseScreenWidget && LoseScreenWidgetClass) { // Create the pause menu widget if it doesn't exist
        LoseScreenWidget = CreateWidget(this, LoseScreenWidgetClass);
    }

    if (LoseScreenWidget) { // Add the widget to the viewport and pause the game
        LoseScreenWidget->AddToViewport();
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        SetPause(true);
    }
}

void AMyPlayerController::DisplayLoseTimeUpScreen()
{
    if (!LoseScreenTimeUpWidget && LoseScreenTimeUpWidgetClass) { // Create the pause menu widget if it doesn't exist
        LoseScreenTimeUpWidget = CreateWidget(this, LoseScreenTimeUpWidgetClass);
    }

    if (LoseScreenTimeUpWidget) { // Add the widget to the viewport and pause the game
        LoseScreenTimeUpWidget->AddToViewport();
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
        SetPause(true);
    }
}

void AMyPlayerController::CallJump()
{
    if (ControlledCharacter) ControlledCharacter->ToJump();
}

void AMyPlayerController::CallStopJumping()
{
    if (ControlledCharacter) ControlledCharacter->ToStopJumping();
}

void AMyPlayerController::CallSprint()
{
    if (ControlledCharacter) ControlledCharacter->Sprint();
}

void AMyPlayerController::CallStopSprinting()
{
    if (ControlledCharacter) ControlledCharacter->StopSprinting();
}

void AMyPlayerController::CallStandToCrouch()
{
    if (ControlledCharacter) ControlledCharacter->StandToCrouch();
}

void AMyPlayerController::CallCrouchToStand()
{
    if (ControlledCharacter) ControlledCharacter->CrouchToStand();
}

void AMyPlayerController::CallForwards(float Value)
{
    if (ControlledCharacter) ControlledCharacter->MoveForward(Value);
}

void AMyPlayerController::CallRight(float Value)
{
    if (ControlledCharacter) ControlledCharacter->MoveRight(Value);
}

void AMyPlayerController::CallTurn(float Value)
{
    if (ControlledCharacter) ControlledCharacter->Turn(Value);
}

void AMyPlayerController::CallLookUp(float Value)
{
    if (ControlledCharacter) ControlledCharacter->LookUp(Value);
}

void AMyPlayerController::CallProne()
{
    if (ControlledCharacter) ControlledCharacter->Prone();
}

void AMyPlayerController::CallFire()
{
    if (ControlledCharacter) ControlledCharacter->Fire();
}

void AMyPlayerController::CallThrowGrenade()
{
    if (ControlledCharacter) ControlledCharacter->ThrowGrenade();
}

void AMyPlayerController::CallOnReloadPressed()
{
    if (ControlledCharacter) ControlledCharacter->Reload();
}

void AMyPlayerController::CallAnimDance()
{
    if (ControlledCharacter) ControlledCharacter->AnimDance();
}

int32 AMyPlayerController::GetCurrentBullets()
{
    return ControlledCharacter->CurrentBulletsInMagazine;
}

int32 AMyPlayerController::GetTotalAmmunition()
{
    return ControlledCharacter->TotalAmmunition;
}

int32 AMyPlayerController::GetGrenadesRemaining()
{
    return ControlledCharacter->GrenadesRemaining;
}

float AMyPlayerController::GetCurrentHealth()
{
    return ControlledCharacter->CurrentHealth;
}

float AMyPlayerController::GetMaxHealth()
{
    return ControlledCharacter->MaxHealth;
}

float AMyPlayerController::GetCurrentStamina()
{
    return ControlledCharacter->CurrentStamina;
}

float AMyPlayerController::GetMaxStamina()
{
    return ControlledCharacter->MaxStamina;
}