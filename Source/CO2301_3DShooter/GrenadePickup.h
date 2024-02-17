// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GrenadePickup.generated.h"

UCLASS()
class CO2301_3DSHOOTER_API AGrenadePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenadePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;

	// Function to handle the overlap event
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// Speed of rotation (degrees per second)
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 90.0f;

	// Speed of vertical movement
	UPROPERTY(EditAnywhere)
	float FloatSpeed = 20.0f;

	// Current time since the game started
	float RunningTime = 0.0f;
};
