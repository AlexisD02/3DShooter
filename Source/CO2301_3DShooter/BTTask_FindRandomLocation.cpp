// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindRandomLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Get the AI controller
    if (AAIController* AIController = OwnerComp.GetAIOwner()) {
        // Get the AI actor
        AActor* AIActor = AIController->GetPawn();
        if (AIActor) {
            // Get the navigation system
            UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIActor);
            if (NavSys) {
                // Define the random search radius
                FNavLocation RandomLocation;

                // Find a random reachable point within the radius
                if (NavSys->GetRandomReachablePointInRadius(AIActor->GetActorLocation(), RandomRadius, RandomLocation)) {
                    // Set the random location in the blackboard
                    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation);
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}