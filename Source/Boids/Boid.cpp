// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "BoidManager.h"

// Sets default values
ABoid::ABoid()
{
	FVector TargetVelocity = FVector::ZeroVector;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	UStaticMesh* SphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh '/Engine/BasicShapes/Sphere.Sphere'")).Object;

	Mesh->SetStaticMesh(SphereMesh);
	this->SetRootComponent(Mesh);
	//BoidManager->FindComponentByClass<ABoidManager>();
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ABoid::Seek(FVector Position)
{
	FVector NewVelocity = Position - GetActorLocation();
	NewVelocity.Normalize();
    return NewVelocity;
}

FVector ABoid::Flee(FVector Position)
{
	FVector NewVelocity = GetActorLocation() - Position;
	NewVelocity.Normalize();
    return NewVelocity;
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoid::UpdateBoid(float DeltaTime)
{
    if (WaitCounter > 0)
    {
        WaitCounter -= DeltaTime;
        return; // Skip updating if waiting
    }

    FVector TargetVelocity = FVector::ZeroVector;

    FVector ClosestBoidPos = BoidManager->ClosestBoidPosition(this);
    if (BoidManager->TaggedBoid == this) // Chase
    {
        TargetVelocity = Seek(ClosestBoidPos);
        TargetVelocity *= BoidManager->ChaseMultiplier;
    }
    else // Flee
    {
        TargetVelocity = Flee(ClosestBoidPos);
    }

    FVector NewForce = TargetVelocity - CurrentVelocity;
    CurrentVelocity += NewForce * DeltaTime;

    FVector Location = GetActorLocation();
    Location += (CurrentVelocity * Speed * DeltaTime);

    SetActorLocation(Location);
}


