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

void AFlockingBoid::BoidSteering(TArray<AFlockingBoid*> Neighbours, float DeltaTime)
{
    FVector TargetVelocity = FVector::ZeroVector;
    if (Neighbours.Num() == 0) {
        bHasNeighbourhood = false;
        TargetVelocity += CurrentVelocity;
    }

        bHasNeighbourhood = true;
        FVector Alignment = FVector::ZeroVector;
        FVector Cohesion = FVector::ZeroVector;
        FVector Sepparation = FVector::ZeroVector;

        for (AFlockingBoid* Boid : Neighbours) {
            if (Boid == this) {
                return;
            }
            FVector BoidLocation = Boid->GetActorLocation();

            FVector Diff = BoidLocation - GetActorLocation();
            float Distance = FVector().Distance(BoidLocation, GetActorLocation());

            if (Distance != 0 && Distance < SeparationValue) {
                Sepparation += Flee(BoidLocation);
            }

            if (Distance < FlockingBoidManager->NeighbourRadius) {
                Cohesion += Diff;
                Alignment += Boid->CurrentVelocity;
            }
        }
        Sepparation.Normalize();
        TargetVelocity += Sepparation * FlockingBoidManager->SeparationWeight;
        TargetVelocity += (Alignment / Neighbours.Num()) * FlockingBoidManager->AllignmentWeight;
        TargetVelocity += Seek(Cohesion * FlockingBoidManager->CohesionWeight);

        // Constrain boid within the sphere
        TargetVelocity = ApplySphereConstraints(TargetVelocity, FlockingBoidManager->SphereCenter, FlockingBoidManager->SphereRadius, FlockingBoidManager->EdgeThreshold);

        FVector NewForce = TargetVelocity - CurrentVelocity;
        CurrentVelocity += NewForce * DeltaTime;
        // Cap the velocity to the maximum allowed speed
        CurrentVelocity = CurrentVelocity.GetClampedToMaxSize(Speed);

        FVector Location = GetActorLocation();
        Location += (CurrentVelocity * Speed * DeltaTime);
        SetActorLocation(Location);

        if (!CurrentVelocity.IsNearlyZero()) // Avoid rotating when velocity is zero
        {
            FRotator NewRotation = CurrentVelocity.Rotation();
            SetActorRotation(NewRotation);
        }
   
    
}

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

// Called every frame
void AFlockingBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    //Debugging neighbour area
    if (bHasNeighbourhood) {
        DrawDebugSphere(GetWorld(),
            GetActorLocation(),
            FlockingBoidManager->NeighbourRadius,
            8,
            FColor::Green,
            false, -1.0f, 0, 1.0f ); 
    } else {
        DrawDebugSphere(GetWorld(),
            GetActorLocation(),
            FlockingBoidManager->NeighbourRadius,
            8,
            FColor::Red,
            false, -1.0f, 0, 1.0f);
    }
    
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
    //FVector TargetVelocity = FVector::ZeroVector;

    TArray<AFlockingBoid*> BoidNeighbourhood = FlockingBoidManager->GetBoidNeighbourhood(this);
    BoidSteering(BoidNeighbourhood, DeltaTime);
    //if (BoidNeighbourhood.Num() > 0) {
    //    bHasNeighbourhood = true;
    //    if (TargetVelocity.Size() < 1.0f) {
    //        TargetVelocity += Sepparation(BoidNeighbourhood) * FlockingBoidManager->SeparationWeight;
    //    }
    //    if (TargetVelocity.Size() < 1.0f) {
    //        TargetVelocity += Cohesion(BoidNeighbourhood) * FlockingBoidManager->CohesionWeight;
    //    }
    //    if (TargetVelocity.Size() < 1.0f) {
    //        TargetVelocity += Alignment(BoidNeighbourhood) * FlockingBoidManager->AllignmentWeight;
    //    }
    //    TargetVelocity.Normalize();

    //    /*if (TargetVelocity.Size() < 1.0f) {
    //        TargetVelocity += Wander(100.0f, 200.0f, 10.0f);
    //        TargetVelocity.Normalize();
    //    }*/
    //}
    //else {
    //    bHasNeighbourhood = false;
    //    TargetVelocity += CurrentVelocity;
    //}
    //
    ////Wanders
    //    

    //// Constrain boid within the sphere
    //TargetVelocity = ApplySphereConstraints(TargetVelocity, FlockingBoidManager->SphereCenter, FlockingBoidManager->SphereRadius, FlockingBoidManager->EdgeThreshold);

    //FVector NewForce = TargetVelocity - CurrentVelocity;
    //CurrentVelocity += NewForce * DeltaTime;

    //FVector Location = GetActorLocation();
    //Location += (CurrentVelocity * Speed * DeltaTime);
    //SetActorLocation(Location);

    //if (!CurrentVelocity.IsNearlyZero()) // Avoid rotating when velocity is zero
    //{
    //    FRotator NewRotation = CurrentVelocity.Rotation();
    //    SetActorRotation(NewRotation);
    //}
}