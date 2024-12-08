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
	for (int i = 0; i < SpawnCount; i++) {
		FVector SpawnLocation = (FMath::VRand() * FMath::RandRange(0.0f, SpawnRadius)) + GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		AFlockingBoid* NewBoid = GetWorld()->SpawnActor<AFlockingBoid>(BBoid, SpawnLocation, SpawnRotation);
		NewBoid->FlockingBoidManager = this;
		MyBoids.Add(NewBoid);
	}

}

// Called every frame
void AFlockingBoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (AFlockingBoid* Boid : MyBoids) {
		Boid->UpdateBoid(DeltaTime);
	}
}

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

FVector AFlockingBoidManager::GetClosestBoidPosition(AFlockingBoid* ThisBoid)
{
    FVector ReturnVal = FVector::ZeroVector;
    float ClosestDistance = 0.0f;
    FString BoidName = "";
    FString ClosestBoidName = "";
    for (AFlockingBoid* Boid : MyBoids) {
        if (Boid == ThisBoid || Boid == NULL ) {
            continue;
        }

        float DistanceToBoid = (ThisBoid->GetActorLocation() - Boid->GetActorLocation()).Size();
        /*UE_LOG(LogTemp, Warning, TEXT("Boid: %s, Distance: %f"), *Boid->GetName(), DistanceToBoid);*/
        if (DistanceToBoid < ClosestDistance) {
            ClosestDistance = DistanceToBoid;
            ReturnVal = Boid->GetActorLocation();
            BoidName = Boid->GetName(); // Correctly assign the FString
            ClosestBoidName = BoidName;
        }
    }

    // WHY DOESN'T THIS LOG????
    UE_LOG(LogTemp, Warning, TEXT("This is the vector: X=%f Y=%f Z=%f"), ReturnVal.X, ReturnVal.Y, ReturnVal.Z);
    UE_LOG(LogTemp, Warning, TEXT("This is the boid being seeked to: %s"), *ClosestBoidName);
    return ReturnVal;
}

