// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"   
#include "GameFramework/Character.h"

AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemyAIController::EnableBehaviorTree()
{
    if (EnemyBehaviorTree) {
        RunBehaviorTree(EnemyBehaviorTree);
    }
}


