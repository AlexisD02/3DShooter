#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "EnemyAIController.h"

AMyGameModeBase::AMyGameModeBase()
{
    GameDuration = 60.0f; // Initialize game duration to 60 seconds
}

void AMyGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()); // Disable input for player controller
    if (PlayerController) PlayerController->DisableInput(PlayerController);

    if (StartSound) UGameplayStatics::PlaySound2D(GetWorld(), StartSound); // Play start sound if available

    if (BackgroundMusic) UGameplayStatics::PlaySound2D(GetWorld(), BackgroundMusic); // Play background music if available

    // Set countdown timer and decrease score timer
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AMyGameModeBase::TakeCountdown, 1.0f, true);
    GetWorldTimerManager().SetTimer(DecreaseScoreTimer, this, &AMyGameModeBase::DecreaseScore, 10.0f, true);
}

void AMyGameModeBase::TakeCountdown()
{
    CurrentCountdownValue--; // Decrease countdown value

    if (CurrentCountdownValue <= 0) StartGame(); 
    // Play appropriate sound based on countdown value
    else UGameplayStatics::PlaySound2D(GetWorld(), (CurrentCountdownValue == 1 && GoCountdownSound) ? GoCountdownSound : CountdownSound);
}

FString AMyGameModeBase::GetCurrentCountdown()
{
    return (CurrentCountdownValue == 1) ? FString("GO!") : FString::FromInt(CurrentCountdownValue - 1); // Return formatted countdown string
}

void AMyGameModeBase::StartGame()
{
    GetWorldTimerManager().ClearTimer(CountdownTimerHandle); // Clear countdown timer

    // Enable behavior tree for enemy controllers
    TArray<AActor*> FoundActors; 
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAIController::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors) {
        AEnemyAIController* EnemyControllerRef = Cast<AEnemyAIController>(Actor);
        if (EnemyControllerRef) {
            EnemyControllerRef->EnableBehaviorTree();
        }
    }

    WinScore = NumEnemiesLeft = FoundActors.Num(); // Get and set the number of enemies and the win score

    // Enable input for player controller and start the game
    AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (PlayerController) {
        PlayerController->EnableInput(PlayerController);
        PlayerController->StartGame();
    }

    // Set game end time and timer
    GameEndTime = GameDuration + GetWorld()->GetTimeSeconds();
    GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AMyGameModeBase::EndGameDueToTime, GameDuration, false);
}

void AMyGameModeBase::ExtendTime()
{
    // Calculate the remaining time left in the current game session
    float RemainingTime = GameEndTime - GetWorld()->GetTimeSeconds();

    // Extend the game duration
    GameDuration += ExtendTimeSec;

    // Update the game end time
    GameEndTime = GetWorld()->GetTimeSeconds() + RemainingTime + ExtendTimeSec;

    // Update the game timer with the new end time and remaining time
    GetWorldTimerManager().ClearTimer(GameTimerHandle);
    GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AMyGameModeBase::EndGameDueToTime, RemainingTime + ExtendTimeSec, false);
}

FString AMyGameModeBase::GetFormattedCountdownTime()
{
    // Calculate the remaining time left in the current game session
    int32 RemainingTime = FMath::Max(0.0f, GameEndTime - GetWorld()->GetTimeSeconds());

    // Calculate minutes and seconds
    int32 Minutes = RemainingTime / 60;
    int32 Seconds = RemainingTime % 60;

    // Format the countdown time
    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}

int32 AMyGameModeBase::CSToDisplay()
{
    return CurrentScoreToDisplay; // Return current score to display
}

void AMyGameModeBase::EnemiesLeft()
{
    NumEnemiesLeft--; // Decrement number of enemies left
}

int32 AMyGameModeBase::GetNumEnemiesLeft()
{
    return NumEnemiesLeft; // Return number of enemies left
}

void AMyGameModeBase::DecreaseScore()
{
    // Decrease score based on timer value
    if (ScoreTimer <= 100 && ScoreTimer >= 80) ScoreTimer -= 10.0f;
    else if (ScoreTimer < 80 && ScoreTimer >= 60) ScoreTimer -= 7.0f;
    else if (ScoreTimer < 60 && ScoreTimer >= 40) ScoreTimer -= 5.0f;
    else if (ScoreTimer < 40 && ScoreTimer >= 20) ScoreTimer -= 3.0f;
    else if (ScoreTimer < 20 && ScoreTimer > 10) ScoreTimer -= 2.0f;
}

void AMyGameModeBase::IncreaseScore()
{
    CurrentScore++; // Increase current score
    CurrentScoreToDisplay += ScoreTimer; // Update current score to display
    CheckGameStatus(); // Check game status for win condition
}

void AMyGameModeBase::CheckGameStatus()
{
    if (CurrentScore >= WinScore) EndGame(true); // If win condition met, end game with player winning
}

void AMyGameModeBase::EndGameDueToTime()
{
    // Get the current world context
    UWorld* World = GetWorld();

    if (World) {
        FName LoseLevelTimeUp = FName(TEXT("/Game/Levels/LoseLevelTimeUp"));
        UGameplayStatics::OpenLevel(World, LoseLevelTimeUp); // Open lose level due to time up
    }
}

void AMyGameModeBase::EndGame(bool bPlayerWon)
{
    FName WinLevel = FName(TEXT("/Game/Levels/WinLevel"));
    FName LoseLevel = FName(TEXT("/Game/Levels/LoseLevel"));
    UGameplayStatics::OpenLevel(this, bPlayerWon ? WinLevel : LoseLevel); // Open win or lose level based on game outcome
}
