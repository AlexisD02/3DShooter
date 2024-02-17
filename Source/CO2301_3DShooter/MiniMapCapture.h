// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "MiniMapCapture.generated.h"

UCLASS()
class CO2301_3DSHOOTER_API AMiniMapCapture : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMiniMapCapture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
