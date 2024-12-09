// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlockingBoidManager.generated.h"


UCLASS()
class BOIDS_API AFlockingBoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlockingBoidManager();
	

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Boid Settings")
	int SpawnCount = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Settings")
	float SpawnRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking Settings")
	float NeighbourRadius = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking Settings")
	float SeparationWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking Settings")
	float CohesionWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking Settings")
	float AllignmentWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Settings")
	TSubclassOf<class AFlockingBoid> BBoid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Settings")
	TArray<class AFlockingBoid*> MyBoids;

	FVector SphereCenter = GetActorLocation(); // Center of the sphere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary Settings")
	float SphereRadius = 1000.0f;                     // Radius of the sphere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary Settings")
	float EdgeThreshold = 100.0f;                     // Threshold for edge avoidance
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<class AFlockingBoid*> GetBoidNeighbourhood(class AFlockingBoid* ThisBoid);
	FVector GetClosestBoidPosition(class AFlockingBoid* ThisBoid, bool bDebugPrint = false);

};
