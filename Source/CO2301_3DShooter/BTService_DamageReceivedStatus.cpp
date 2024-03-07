// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DamageReceivedStatus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"

void UBTService_DamageReceivedStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return;
    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
    if (!EnemyCharacter) return;

    // Check if damage was received
    if (EnemyCharacter->bDamageReceived) {
        // Set the blackboard boolean value to true
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);

        // Reset the damage received flag
        EnemyCharacter->bDamageReceived = false;
    }
    else {
        // Clear the blackboard boolean value if no damage is received
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
    }
}