// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create subobjects for the different components.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    HealthBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBoxMesh"));
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

    // Set default values for the components.
    HealthBoxMesh->SetupAttachment(RootComponent);
    CollisionBox->SetupAttachment(HealthBoxMesh);
    CollisionBox->SetBoxExtent(FVector(500.0f, 500.0f, 500.0f)); // Adjust the size as needed
    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
    Super::BeginPlay();

    // Register Events
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickup::OnOverlapBegin);
}

// Called every frame
void AHealthPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); // Call parent class tick function

    // Rotate the actor
    AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));

    // Float the actor up and down
    FVector NewLocation = GetActorLocation();
    float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
    NewLocation.Z += DeltaHeight * FloatSpeed; // FloatSpeed controls the speed of the floating
    SetActorLocation(NewLocation);

    RunningTime += DeltaTime; // Update running time
}

void AHealthPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);

    if (MyCharacter && (MyCharacter->CurrentHealth < MyCharacter->MaxHealth)) {
        MyCharacter->AddHealth(); // Assume AddHealth takes an amount parameter
        Destroy(); // Destroy the health box if health was added
    }
}