// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingBoid.h"
#include "FlockingBoidManager.h"

// Sets default values
AFlockingBoid::AFlockingBoid()
{
	FVector TargetVelocity = FVector::ZeroVector;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlockingBoid::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AFlockingBoid::Alignment(TArray<AFlockingBoid*> Neighbours)
{
    if (Neighbours.Num() == 0) {
        return FVector::ZeroVector;
    }

    FVector NewVelocity;
    for (AFlockingBoid* Boid : Neighbours) {
        NewVelocity += Boid->CurrentVelocity;
    }
    NewVelocity /= Neighbours.Num();
    NewVelocity.Normalize();
    return NewVelocity;
}

FVector AFlockingBoid::Cohesion(TArray<AFlockingBoid*> Neighbours)
{
    if (Neighbours.Num() == 0) {
        return FVector::ZeroVector;
    }

    FVector CohesionPoint;

    float InverseVal = 1 / Neighbours.Num();

    for (AFlockingBoid* Boid : Neighbours) {
        CohesionPoint += (Boid->GetActorLocation() * InverseVal);
    }

    return Seek(CohesionPoint);
}

FVector AFlockingBoid::Sepparation(TArray<AFlockingBoid*> Neighbours)
{
    if (Neighbours.Num() == 0) {
        return FVector::ZeroVector;
    }

    FVector FleePoint;

    float InverseVal = 1 / Neighbours.Num();
    for (AFlockingBoid* Boid : Neighbours) {
        FleePoint += Flee(Boid->GetActorLocation());
    }
    FleePoint.Normalize();
    return FleePoint;
}

// Called every frame
void AFlockingBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
FVector AFlockingBoid::Seek(FVector Position)
{
	FVector NewVelocity = Position - GetActorLocation();
	NewVelocity.Normalize();
	return NewVelocity;
}

FVector AFlockingBoid::Flee(FVector Position)
{
	FVector NewVelocity = GetActorLocation() - Position;
	NewVelocity.Normalize();
	return NewVelocity;
}

FVector AFlockingBoid::Wander(float Radius, float Distance, float Jitter)
{
    FVector MyLocation = GetActorLocation();
    if (FVector::Dist(MyLocation, WanderDestination) < 100.0f) {
        FVector ProjectedPosition = MyLocation + (GetActorForwardVector() * Distance);

        WanderDestination = ProjectedPosition + (FMath::Rand() * FMath::RandRange(0.0f, Jitter));
    }

    FVector JitterDestination = Seek(WanderDestination) + (FMath::VRand() * FMath::RandRange(0.0f, Jitter));
    return JitterDestination;
}

void AFlockingBoid::UpdateBoid(float DeltaTime)
{
    FVector TargetVelocity = FVector::ZeroVector;

    TArray<AFlockingBoid*> ClosestBoidsPositions = FlockingBoidManager->GetBoidNeighbourhood(this);

    TargetVelocity += Sepparation(ClosestBoidsPositions) * FlockingBoidManager->SeparationWeight;
    TargetVelocity += Cohesion(ClosestBoidsPositions) * FlockingBoidManager->CohesionWeight;
    TargetVelocity += Alignment(ClosestBoidsPositions) * FlockingBoidManager->AllignmentWeight;

    TargetVelocity.Normalize();
    
    if (TargetVelocity.Size() < 1.0f) {
        TargetVelocity += Wander(100.0f, 200.0f, 50.0f);
        TargetVelocity.Normalize();
    }
    FVector NewForce = TargetVelocity - CurrentVelocity;
    CurrentVelocity += NewForce * DeltaTime;

    FVector Location = GetActorLocation();
    Location += (CurrentVelocity * Speed * DeltaTime);

    SetActorLocation(Location);
}