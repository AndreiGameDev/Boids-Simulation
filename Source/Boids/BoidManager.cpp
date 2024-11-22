// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"
#include "Boid.h"
// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Transform = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	this->SetRootComponent(Transform);
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < SpawnCount; i++) {
		FVector SpawnLocation = (FMath::VRand() * SpawnRadius) + GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		ABoid* NewBoid = GetWorld()->SpawnActor<ABoid>(SpawnLocation, SpawnRotation);
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

