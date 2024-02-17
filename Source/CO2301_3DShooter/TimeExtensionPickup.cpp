// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeExtensionPickup.h"
#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATimeExtensionPickup::ATimeExtensionPickup()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create subobjects for the different components.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    ClockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockMesh"));
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

    // Set default values for the components.
    ClockMesh->SetupAttachment(RootComponent);
    CollisionBox->SetupAttachment(ClockMesh);
    CollisionBox->SetBoxExtent(FVector(1.5f, 1.5f, 1.5f)); // Adjust the size as needed
    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ATimeExtensionPickup::BeginPlay()
{
    Super::BeginPlay();

    // Register Events
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATimeExtensionPickup::OnOverlapBegin);
}

// Called every frame
void ATimeExtensionPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); // Call parent class tick function

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

void ATimeExtensionPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
    AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameMode && MyCharacter) {
        GameMode->ExtendTime();
        Destroy();
    }
}