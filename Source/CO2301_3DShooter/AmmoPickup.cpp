// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

// Sets default values
AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create subobjects for the different components.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    AmmoBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoBoxMesh"));
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

    // Set default values for the components.
    AmmoBoxMesh->SetupAttachment(RootComponent);
    CollisionBox->SetupAttachment(AmmoBoxMesh);
    CollisionBox->SetBoxExtent(FVector(500.0f, 500.0f, 500.0f)); // Adjust the size as needed
    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	
    // Register Events
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnOverlapBegin);
}

// Called every frame
void AAmmoPickup::Tick(float DeltaTime)
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

void AAmmoPickup::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);

    if (MyCharacter) {
        MyCharacter->AddAmmo();
        Destroy();
    }
}



