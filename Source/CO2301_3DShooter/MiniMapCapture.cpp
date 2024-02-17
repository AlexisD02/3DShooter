// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapCapture.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMiniMapCapture::AMiniMapCapture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMiniMapCapture::BeginPlay()
{
	Super::BeginPlay();
}

