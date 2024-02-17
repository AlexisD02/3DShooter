// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TriggerAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include "AIController.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UBTTask_TriggerAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;

    AEnemyCharacter* AIPawn = Cast<AEnemyCharacter>(AIController->GetPawn());
    if (!AIPawn) return EBTNodeResult::Failed;

    AIPawn->Fire(); // Call the Fire method

    return EBTNodeResult::Succeeded;
}
