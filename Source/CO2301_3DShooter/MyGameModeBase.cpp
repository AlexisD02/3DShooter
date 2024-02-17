#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
    // Initialize default values
    ChallengeLevel = 1;
    GameDuration = 60.0f; // 1 minutes game duration
}

void AMyGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Play the start sound
    if (StartSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), StartSound);
    }

    if (BackgroundMusic != nullptr)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), BackgroundMusic);
    }

    // Call the method to initialize enemies and other game elements
    AEnemyCharacter::InitializeEnemyCountForGameMode(GetWorld());

    DisableAllCharacterInput();
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AMyGameModeBase::TakeCountdown, 1.0f, true);
}

void AMyGameModeBase::DisableAllCharacterInput()
{
    AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (PlayerController)
    {
        // Disable input for this character's controller
        PlayerController->DisableInput(PlayerController);
    }
}

void AMyGameModeBase::TakeCountdown()
{
    CurrentCountdownValue--;

    if (CurrentCountdownValue <= 0) {
        // If countdown reaches zero, start the game
        StartGame();
    }
    else {
        if (CurrentCountdownValue == 1) {
            if (GoCountdownSound) {
                UGameplayStatics::PlaySound2D(GetWorld(), GoCountdownSound);
            }
        }
        else {
            // Play the start sound
            if (CountdownSound) {
                UGameplayStatics::PlaySound2D(GetWorld(), CountdownSound);
            }
        }
    }
}

FString AMyGameModeBase::GetCurrentCountdown()
{
    ValueToDisplay = CurrentCountdownValue - 1;
    if (ValueToDisplay <= 0)
    {
        return FString("GO!");
    }
    else
    {
        // Convert int32 to FString
        return FString::FromInt(ValueToDisplay);
    }
}

void AMyGameModeBase::StartGame()
{
    GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAIController::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors) {
        AEnemyAIController* EnemyControllerRef = Cast<AEnemyAIController>(Actor);
        if (EnemyControllerRef) {
            // Assuming EnableBehaviorTree is a function in AEnemyAIController
            EnemyControllerRef->EnableBehaviorTree();
        }
    }

    AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (PlayerController) {
        // Disable input for this character's controller
        PlayerController->EnableInput(PlayerController);
        PlayerController->StartGame();
    }

    // Start the main game timer
    GameTimer();
}

void AMyGameModeBase::SetWinScore(int32 EnemyCount)
{
    WinScore = EnemyCount;
    NumEnemiesLeft = EnemyCount;
    // Optionally, log the new WinScore for debugging
    UE_LOG(LogTemp, Warning, TEXT("WinScore set to: %d"), WinScore);
}

void AMyGameModeBase::EnemiesLeft()
{
    NumEnemiesLeft--;
    UE_LOG(LogTemp, Warning, TEXT("WinScore set to: %d"), NumEnemiesLeft);
}


void AMyGameModeBase::GameTimer()
{
    GameEndTime = GetWorld()->GetTimeSeconds() + GameDuration;
    GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AMyGameModeBase::EndGameDueToTime, GameDuration, false);
}

void AMyGameModeBase::UpdateGameTimer()
{
    // Calculate the end time of the game
    GameEndTime = GetWorld()->GetTimeSeconds() + GameDuration;

    // Calculate the remaining time
    float RemainingTime = GameEndTime - GetWorld()->GetTimeSeconds();

    // Cancel the existing timer and set a new one with the updated remaining time
    GetWorldTimerManager().ClearTimer(GameTimerHandle);
    GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AMyGameModeBase::EndGameDueToTime, RemainingTime, false);

    // You can use the remaining time as needed, for example, print it to the console
    UE_LOG(LogTemp, Warning, TEXT("Remaining Time: %f seconds"), RemainingTime);
}

FString AMyGameModeBase::GetFormattedCountdownTime()
{
    int32 RemainingTime = FMath::Max(0.0f, GameEndTime - GetWorld()->GetTimeSeconds());
    int32 Minutes = RemainingTime / 60;
    int32 Seconds = RemainingTime % 60;
    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}

int32 AMyGameModeBase::GetNumEnemiesLeft()
{
    return NumEnemiesLeft;
}

void AMyGameModeBase::IncreaseScore()
{
    CurrentScore++;
    CheckGameStatus();
}

void AMyGameModeBase::IncreaseChallenge()
{
    ChallengeLevel++;
    // Implement logic for increasing game difficulty here
}

void AMyGameModeBase::ExtendTime()
{
    float RemainingTime = GameEndTime - GetWorld()->GetTimeSeconds();
    GameDuration = RemainingTime + ExtendTimeSec;
    UpdateGameTimer();
}

void AMyGameModeBase::CheckGameStatus()
{
    if (CurrentScore >= WinScore) {
        EndGame(true); // Player wins
    }
    // No else, as losing conditions are checked elsewhere
}

void AMyGameModeBase::EndGameDueToTime()
{
    UGameplayStatics::OpenLevel(GetWorld(), "LoseLevelTimeUp");
    GetWorldTimerManager().ClearTimer(GameTimerHandle);
}

void AMyGameModeBase::MainCharacterKilled()
{
    EndGame(false); // Player loses
}

void AMyGameModeBase::EndGame(bool bPlayerWon)
{
    if (bPlayerWon) {
        UGameplayStatics::OpenLevel(GetWorld(), "WinLevel");
    }
    else {
        UGameplayStatics::OpenLevel(GetWorld(), "LoseLevel");
    }
    // Clear the game timer if it's still running
    GetWorldTimerManager().ClearTimer(GameTimerHandle);
}