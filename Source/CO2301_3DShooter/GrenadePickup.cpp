// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenadePickup.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h" // Include your character header

// Sets default values
AGrenadePickup::AGrenadePickup()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize components
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

    // Setup the component hierarchy
    GrenadeMesh->SetupAttachment(RootComponent);
    CollisionBox->SetupAttachment(GrenadeMesh);
    CollisionBox->SetBoxExtent(FVector(10.0f, 10.0f, 10.0f)); // Adjust based on grenade size
    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AGrenadePickup::BeginPlay()
{
    Super::BeginPlay();

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGrenadePickup::OnOverlapBegin);
}

// Called every frame
void AGrenadePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Rotate the actor
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += RotationSpeed * DeltaTime;
    SetActorRotation(NewRotation);

    // Float the actor up and down
    FVector NewLocation = GetActorLocation();
    float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
    NewLocation.Z += DeltaHeight * FloatSpeed; // FloatSpeed controls the speed of the floating
    SetActorLocation(NewLocation);

    RunningTime += DeltaTime; // Update running time
}

void AGrenadePickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);

    if (MyCharacter) {
        // Assuming AddGrenade is a function you've implemented on your character
        MyCharacter->AddGrenade();
        Destroy();
    }
}
