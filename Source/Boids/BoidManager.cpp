// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"
#include "Boid.h"



// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < SpawnCount; i++) {
		FVector SpawnLocation = (FMath::VRand() * SpawnRadius) + GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		ABoid* NewBoid = GetWorld()->SpawnActor<ABoid>(BBoid, SpawnLocation, SpawnRotation);
		NewBoid->BoidManager = this;
		MyBoids.Add(NewBoid);
	}

		TaggedBoid = MyBoids[0];
}


// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (ABoid* Boid : MyBoids) {
		Boid->UpdateBoid(DeltaTime);
	}
}

FVector ABoidManager::ClosestBoidPosition(ABoid* ThisBoid)
{

    float ClosestDistance = MaxFleeDistance;
    FVector ReturnVal = FVector::ZeroVector;

    for (ABoid* Boid : MyBoids)
    {
        if (Boid == ThisBoid || Boid == LastTagged)
        {
            continue;
        }

        float Distance = (Boid->GetActorLocation() - ThisBoid->GetActorLocation()).Size();
        if (Distance < ClosestDistance)
        {
            ClosestDistance = Distance;
            ReturnVal = Boid->GetActorLocation();
        }

        if (ThisBoid == TaggedBoid && ClosestDistance < CatchRange)
        {
            LastTagged = TaggedBoid;
            TaggedBoid = Boid;
            TaggedBoid->WaitCounter = TimeoutTime;
        }
    }

    return ReturnVal;
}



