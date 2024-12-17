// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingBoid.h"
#include "FlockingBoidManager.h"
#include "Logging/StructuredLog.h"
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

// Constraints boid movement within a sphere boundary.
FVector AFlockingBoid::ApplySphereConstraints(FVector CurrentActorVelocity, FVector SphereCenter, float SphereRadius, float EdgeThreshold)
{
    FVector ToCenter = SphereCenter - GetActorLocation();
    float DistanceToCenter = ToCenter.Size();

    // If the boid is outside the sphere
    if (DistanceToCenter > SphereRadius)
    {
        FVector CorrectionForce = ToCenter.GetSafeNormal() * (DistanceToCenter - SphereRadius);
        CurrentActorVelocity += CorrectionForce; // Push back towards center
    }
    // If boid is near the edge of the sphere
    else if (SphereRadius - DistanceToCenter < EdgeThreshold)
    {
        FVector AvoidEdgeForce = ToCenter.GetSafeNormal() * (EdgeThreshold - (SphereRadius - DistanceToCenter));
        CurrentActorVelocity += AvoidEdgeForce; // Gently steer away from edge
    }

    return CurrentActorVelocity.GetClampedToMaxSize(Speed);
}

// Debugs neighbour hood radius by displaying a sphere around the boid.
void AFlockingBoid::DebugNeighbourRadius()
{
    //Debugging neighbour area
    if (bHasNeighbourhood) {
        DrawDebugSphere(GetWorld(),
            GetActorLocation(),
            FlockingBoidManager->NeighbourRadius,
            8,
            FColor::Green,
            false, -1.0f, 0, 1.0f);
    }
    else {
        DrawDebugSphere(GetWorld(),
            GetActorLocation(),
            FlockingBoidManager->NeighbourRadius,
            8,
            FColor::Red,
            false, -1.0f, 0, 1.0f);
    }
}

// Called every frame
void AFlockingBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (FlockingBoidManager->bDebugNeighbourHood) {
        DebugNeighbourRadius();
    }
    
}
FVector AFlockingBoid::Seek(FVector Position)
{
    FVector NewVelocity = Position - GetActorLocation();
    NewVelocity.Normalize();
    return NewVelocity;
}

void AFlockingBoid::UpdateBoid(float DeltaTime)
{
    TArray<AFlockingBoid*> Neighbours = FlockingBoidManager->GetBoidNeighbourhood(this);

    if (Neighbours.Num() == 0)
    {
        bHasNeighbourhood = false;
        FVector TargetVelocity = Seek(FlockingBoidManager->GetClosestBoidPosition(this));
        // Set sphere boundary for boids
        TargetVelocity = ApplySphereConstraints(TargetVelocity, FlockingBoidManager->SphereCenter, FlockingBoidManager->SphereRadius, FlockingBoidManager->EdgeThreshold);

        // Cap velocity to max speed
        TargetVelocity = TargetVelocity.GetClampedToMaxSize(Speed);


        // Apply new force
        FVector NewForce = TargetVelocity - CurrentVelocity;
        CurrentVelocity += NewForce * DeltaTime;

        // Update position
        FVector NewLocation = GetActorLocation() + (CurrentVelocity * Speed * DeltaTime);
        SetActorLocation(NewLocation);

        // Update rotation
        if (!CurrentVelocity.IsNearlyZero())
        {
            FRotator NewRotation = CurrentVelocity.Rotation();
            SetActorRotation(NewRotation);
        }
        return;
    }

    bHasNeighbourhood = true;

    // Initialize forces
    FVector AlignmentForce = FVector::ZeroVector;
    FVector CohesionForce = FVector::ZeroVector;
    FVector SeparationForce = FVector::ZeroVector;

    // Iterate through neighbors
    for (AFlockingBoid* Boid : Neighbours)
    {
        if (Boid == this) continue;

        FVector Diff = Boid->GetActorLocation() - GetActorLocation();
        float Distance = Diff.Size();

        // Separation force
        if (Distance > 0 && Distance < SeparationValue)
        {
            SeparationForce += (-Diff / Distance); // Invert and scale by distance
        }

        // Cohesion and Alignment within Neighbor Radius
        if (Distance < FlockingBoidManager->NeighbourRadius)
        {
            CohesionForce += Diff;                // Cohesion: move towards neighbors
            AlignmentForce += Boid->CurrentVelocity; // Alignment: match neighbors' velocity
        }
    }

    // Normalize forces and scale by weights
    if (!SeparationForce.IsNearlyZero())
    {
        SeparationForce.Normalize();
        SeparationForce *= FlockingBoidManager->SeparationWeight;
    }

    if (!AlignmentForce.IsNearlyZero())
    {
        AlignmentForce /= Neighbours.Num(); // Average alignment
        AlignmentForce.Normalize();
        AlignmentForce *= FlockingBoidManager->AllignmentWeight;
    }

    if (!CohesionForce.IsNearlyZero())
    {
        CohesionForce.Normalize();
        CohesionForce *= FlockingBoidManager->CohesionWeight;
    }

    // Combine forces
    FVector TargetVelocity = SeparationForce + AlignmentForce + CohesionForce;

    // Set sphere boundary for boids
    TargetVelocity = ApplySphereConstraints(TargetVelocity, FlockingBoidManager->SphereCenter, FlockingBoidManager->SphereRadius, FlockingBoidManager->EdgeThreshold);

    // Cap velocity to max speed
    TargetVelocity = TargetVelocity.GetClampedToMaxSize(Speed);


    // Apply new force
    FVector NewForce = TargetVelocity - CurrentVelocity;
    CurrentVelocity += NewForce * DeltaTime;

    // Update position
    FVector NewLocation = GetActorLocation() + (CurrentVelocity * Speed * DeltaTime);
    SetActorLocation(NewLocation);

    // Update rotation
    if (!CurrentVelocity.IsNearlyZero())
    {
        FRotator NewRotation = CurrentVelocity.Rotation();
        SetActorRotation(NewRotation);
    }

}