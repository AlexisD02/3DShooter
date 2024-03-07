// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAIController.generated.h"

/**
 *
 */
UCLASS()
class CO2301_3DSHOOTER_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

    // Enable the behavior tree
    void EnableBehaviorTree();

protected:
    // Reference to the Behavior Tree component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* EnemyBehaviorTree;
};
