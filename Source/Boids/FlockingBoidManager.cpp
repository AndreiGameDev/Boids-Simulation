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
		FVector SpawnLocation = (FMath::VRand() + FMath::RandRange(0.0f, SpawnRadius)) + GetActorLocation();
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

