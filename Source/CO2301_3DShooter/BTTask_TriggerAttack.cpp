// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TriggerAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include "AIController.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UBTTask_TriggerAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the AI controller and check if it's an enemy AI controller
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;

    // Get the AI pawn and check if it's an enemy character
    AEnemyCharacter* AIPawn = Cast<AEnemyCharacter>(AIController->GetPawn());
    if (!AIPawn) return EBTNodeResult::Failed;

    // Call the Fire method of the enemy character
    AIPawn->Fire();

    return EBTNodeResult::Succeeded;
}