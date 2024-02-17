// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_LineOfSight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

//LN2 Implement TickNode. Notice that OwnerComp is passing a reference and not a pointer - it is already dereferenced.
void UBTService_LineOfSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) {
        return;
    }

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    APawn* AIPawn = AIController->GetPawn();
    if (!PlayerPawn || !AIPawn) {
        return;
    }

    FVector EnemyLocation = AIPawn->GetActorLocation();
    FVector PlayerLocation = PlayerPawn->GetActorLocation();
    float DistanceToPlayer = FVector::Dist(EnemyLocation, PlayerLocation);

    const float ViewDistance = 2000.0f; // Set this to whatever your view distance should be

    if (DistanceToPlayer <= ViewDistance && AIController->LineOfSightTo(PlayerPawn)) {
        FVector DirectionToPlayer = (PlayerLocation - EnemyLocation).GetSafeNormal();
        FVector EnemyForward = AIPawn->GetActorForwardVector();

        if (FVector::DotProduct(EnemyForward, DirectionToPlayer) > 0) {
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
        }
        else {
            OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        }
    }
    else {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}
