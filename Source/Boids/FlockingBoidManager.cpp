// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingBoidManager.h"
#include "FlockingBoid.h"


// Sets default values
AFlockingBoidManager::AFlockingBoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AFlockingBoidManager::BeginPlay()
{
	Super::BeginPlay();
    

}

// Called every frame
void AFlockingBoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (bDebugBoundary) {
        // Draws the edge of the sphere
        DrawDebugSphere(GetWorld(),
            SphereCenter,
            SphereRadius,
            32,
            FColor::Blue,
            false, -1.0f, 0, 1.0f
        );
    }
    if (bDebugEdgeTreshhold) {
        // Draws the sphere of how far from the edge the boids should turn
        DrawDebugSphere(GetWorld(),
            SphereCenter,
            SphereRadius - EdgeThreshold,
            32,
            FColor::Yellow,
            false, -1.0f, 0, 1.0f
        );
    }

	for (AFlockingBoid* Boid : MyBoids) {
		Boid->UpdateBoid(DeltaTime);
	}
}

// Grabs every boid within a certain range and creates a neighbourhood.
TArray<class AFlockingBoid*> AFlockingBoidManager::GetBoidNeighbourhood(AFlockingBoid* ThisBoid)
{
    TArray<class AFlockingBoid*> ReturnBoids;

    for (AFlockingBoid* Boid : MyBoids)
    {
        if (Boid == ThisBoid || !Boid)
        {
            continue;
        }

        float Distance = (Boid->GetActorLocation() - ThisBoid->GetActorLocation()).Size();
        if (Distance < NeighbourRadius)
        {
            ReturnBoids.Add(Boid);
        }
	}
	return ReturnBoids;
}

void AFlockingBoidManager::SetProperties(const FBoidManagerProperties& NewProperties)
{
    SpawnCount = NewProperties.SpawnCount;
    SpawnRadius = NewProperties.SpawnRadius;
    NeighbourRadius = NewProperties.NeighbourRadius;
    SeparationWeight = NewProperties.SeparationWeight;
    CohesionWeight = NewProperties.CohesionWeight;
    AllignmentWeight = NewProperties.AllignmentWeight;
    SphereRadius = NewProperties.SphereRadius;
    EdgeThreshold = NewProperties.EdgeThreshold;
    bDebugBoundary = NewProperties.bDebugBoundary;
    bDebugEdgeTreshhold = NewProperties.bDebugEdgeTreshhold;
    bDebugNeighbourHood = NewProperties.bDebugNeighbourHood;
}


FBoidManagerProperties AFlockingBoidManager::GetProperties()
{
    FBoidManagerProperties Properties;

    Properties.SpawnCount = SpawnCount;
    Properties.SpawnRadius = SpawnRadius;
    Properties.NeighbourRadius = NeighbourRadius;
    Properties.SeparationWeight = SeparationWeight;
    Properties.CohesionWeight = CohesionWeight;
    Properties.AllignmentWeight = AllignmentWeight;
    Properties.SphereRadius = SphereRadius;
    Properties.EdgeThreshold = EdgeThreshold;
    Properties.bDebugBoundary = bDebugBoundary;
    Properties.bDebugEdgeTreshhold = bDebugEdgeTreshhold;
    Properties.bDebugNeighbourHood = bDebugNeighbourHood;

    return Properties;
}

// Spawns every boid according to spawn count
void AFlockingBoidManager::SpawnBoids(bool& BDebugOutput, FString& DebugOutputText)
{
    for (int i = 0; i < SpawnCount; i++) {
        FVector SpawnLocation = (FMath::VRand() * FMath::RandRange(0.0f, SpawnRadius)) + GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();

        AFlockingBoid* NewBoid = GetWorld()->SpawnActor<AFlockingBoid>(BBoid, SpawnLocation, SpawnRotation);
        NewBoid->FlockingBoidManager = this;
        MyBoids.Add(NewBoid);
        IgnoreBoidsArray.Add(NewBoid);
    }
    BDebugOutput = true;
    DebugOutputText = FString::Printf(TEXT("Initialised %d boids"), MyBoids.Num());
}

// Destroys the boids then empties the array.
void AFlockingBoidManager::DespawnBoids(bool& BDebugOutput, FString& DebugOutputText)
{
    for (AFlockingBoid* Boid : MyBoids) {
        Boid->Destroy();
    }
    MyBoids.Empty();
    IgnoreBoidsArray.Empty();
    BDebugOutput = true;
    DebugOutputText = FString::Printf(TEXT("Destroyed every boid, Myboids count is at %f"), MyBoids.Num());
}

// Itterates through every boid and compares distances to find the closest boid position compared to the boid fed into the function.
FVector AFlockingBoidManager::GetClosestBoidPosition(AFlockingBoid* ThisBoid, bool bDebugPrint)
{
    FVector ReturnVal = FVector::ZeroVector;
    float ClosestDistance = FLT_MAX;
    FString BoidName = "";
    for (AFlockingBoid* Boid : MyBoids) {
        if (Boid == ThisBoid || Boid == NULL ) {
            continue;
        }
        float DistanceToBoid = (ThisBoid->GetActorLocation() - Boid->GetActorLocation()).Size();
        if (DistanceToBoid < ClosestDistance) {
            ClosestDistance = DistanceToBoid;
            ReturnVal = Boid->GetActorLocation();
            BoidName = *Boid->GetName();
        }
    }
    if (bDebugPrint) {
        UE_LOG(LogTemp, Warning, TEXT("%s is seeking %s"),*ThisBoid->GetName(), *BoidName);
    }
    
    return ReturnVal;
}
