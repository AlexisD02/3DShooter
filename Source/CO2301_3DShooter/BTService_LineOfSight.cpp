// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_LineOfSight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

void UBTService_LineOfSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // Get the AI controller
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    // Get the player pawn and AI pawn
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    APawn* AIPawn = AIController->GetPawn();
    if (!PlayerPawn || !AIPawn) return;

    // Calculate distance between AI and player
    FVector EnemyLocation = AIPawn->GetActorLocation();
    FVector PlayerLocation = PlayerPawn->GetActorLocation();
    float DistanceToPlayer = FVector::Dist(EnemyLocation, PlayerLocation);

    // Check if player is within view distance and there's line of sight to the player
    if (DistanceToPlayer <= ViewDistance && AIController->LineOfSightTo(PlayerPawn)) {
        // Get direction to player and AI's forward vector
        FVector DirectionToPlayer = (PlayerLocation - EnemyLocation).GetSafeNormal();
        FVector EnemyForward = AIPawn->GetActorForwardVector();

        // Check if player is within AI's field of view
        if (FVector::DotProduct(EnemyForward, DirectionToPlayer) > 0) {
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
        }
        else {
            // Clear blackboard value if player is not in AI's field of view
            OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        }
    }
    else {
        // Clear blackboard value if player is out of view distance or there's no line of sight
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}
