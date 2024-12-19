// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingBoid.h"
#include "FlockingBoidManager.h"
#include "Logging/StructuredLog.h"
#include "Kismet/KismetSystemLibrary.h"

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

// Debugs neighbour hood radius by displaying a sphere around the boid, it will turn green if it has a neighbourhood, red if it doesn't.
void AFlockingBoid::DebugNeighbourRadius()
{
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

// Mathematical equation to get the vector to seek towards a position.
FVector AFlockingBoid::Seek(FVector Position)
{
    FVector NewVelocity = Position - GetActorLocation();
    NewVelocity.Normalize();
    return NewVelocity;
}

FVector AFlockingBoid::CollisionAvoidance()
{
    TArray<FHitResult> Hits;
    EDrawDebugTrace::Type TraceType = EDrawDebugTrace::None;
    if (FlockingBoidManager->bDebugObjectAvoidance) {
        TraceType = EDrawDebugTrace::ForOneFrame;
    }
    bool bHit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
        GetActorLocation() + ObjectAvoidanceRange,
        ObjectAvoidanceRadius,
        UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), true,
        FlockingBoidManager->IgnoreBoidsArray,
        TraceType,
        Hits,
        true, 
        FLinearColor::Blue);
    if (bHit) {
        float Distance = FVector::Distance(GetActorLocation(), Hits[0].Location);
        return Flee(Hits[0].Location + Distance);
    }
    else {
        return CurrentVelocity;
    }

    
}

FVector AFlockingBoid::Flee(FVector Position)
{
    FVector NewVelocity = GetActorLocation() - Position;
    NewVelocity.Normalize();
    return NewVelocity;
}
// Steering and the constraining of the boid happens here.
void AFlockingBoid::UpdateBoid(float DeltaTime)
{
    TArray<AFlockingBoid*> Neighbours = FlockingBoidManager->GetBoidNeighbourhood(this);
   
    // If there is neighbourhood we'll try to seek towards one.
    if (Neighbours.Num() == 0)
    {
        bHasNeighbourhood = false;
        FVector TargetVelocity = CollisionAvoidance();
        TargetVelocity = Seek(FlockingBoidManager->GetClosestBoidPosition(this));
        TargetVelocity = ApplySphereConstraints(TargetVelocity, FlockingBoidManager->SphereCenter, FlockingBoidManager->SphereRadius, FlockingBoidManager->EdgeThreshold);
        TargetVelocity = TargetVelocity.GetClampedToMaxSize(Speed);

        FVector NewForce = TargetVelocity - CurrentVelocity;
        CurrentVelocity += NewForce * DeltaTime;
        FVector NewLocation = GetActorLocation() + (CurrentVelocity * Speed * DeltaTime);
        SetActorLocation(NewLocation);
        if (!CurrentVelocity.IsNearlyZero())
        {
            FRotator NewRotation = CurrentVelocity.Rotation();
            SetActorRotation(NewRotation);
        }
        return;
    }

    bHasNeighbourhood = true;

    FVector AlignmentForce = FVector::ZeroVector;
    FVector CohesionForce = FVector::ZeroVector;
    FVector SeparationForce = FVector::ZeroVector;

    // We calculate the force maths in this section and iterate through boids once to save resources.
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
    FVector TargetVelocity = CollisionAvoidance();
    // Combine forces
    TargetVelocity += SeparationForce + AlignmentForce + CohesionForce;
    // Set sphere boundary for boids and cap the velocity
    TargetVelocity = ApplySphereConstraints(TargetVelocity, FlockingBoidManager->SphereCenter, FlockingBoidManager->SphereRadius, FlockingBoidManager->EdgeThreshold);
    TargetVelocity = TargetVelocity.GetClampedToMaxSize(Speed);
    FVector NewForce = TargetVelocity - CurrentVelocity;
    CurrentVelocity += NewForce * DeltaTime;
    //Setting location and rotation
    FVector NewLocation = GetActorLocation() + (CurrentVelocity * Speed * DeltaTime);
    SetActorLocation(NewLocation);
    if (!CurrentVelocity.IsNearlyZero())
    {
        FRotator NewRotation = CurrentVelocity.Rotation();
        SetActorRotation(NewRotation);
    }
}