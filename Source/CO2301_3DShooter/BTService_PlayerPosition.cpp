// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerPosition.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void UBTService_PlayerPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // Get the player's pawn
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // Set the player's position in the blackboard
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
}

