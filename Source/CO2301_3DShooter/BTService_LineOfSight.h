// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_LineOfSight.generated.h"

/**
 * 
 */
UCLASS()
class CO2301_3DSHOOTER_API UBTService_LineOfSight : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	const float ViewDistance = 2500.0f; // Set this to whatever your view distance should be
};
