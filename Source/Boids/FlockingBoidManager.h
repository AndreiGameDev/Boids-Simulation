// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlockingBoidManager.generated.h"

USTRUCT(BlueprintType)
struct FBoidManagerProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Manager Settings")
	int SpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Manager Settings")
	float SpawnRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Manager Settings")
	float NeighbourRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Manager Settings")
	float SeparationWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Manager Settings")
	float CohesionWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Manager Settings")
	float AllignmentWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Manager Settings")
	float SphereRadius;   
                
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Manager Settings")
	float EdgeThreshold;

	// Default constructor for initialization
	FBoidManagerProperties()
		: SpawnCount(30)
		, SpawnRadius(500.0f)
		, NeighbourRadius(900.0f)
		, SeparationWeight(1.0f)
		, CohesionWeight(1.0f)
		, AllignmentWeight(1.0f)
		, SphereRadius(1000.0f)
		, EdgeThreshold(100.0f)
	{
	}
};


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Settings")
	bool bNeighbourRadius = false;

	FVector SphereCenter = GetActorLocation(); // Center of the sphere
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary Settings")
	float SphereRadius = 1000.0f;                     // Radius of the sphere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary Settings")
	float EdgeThreshold = 100.0f;                     // Threshold for edge avoidance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary Settings")
	bool bDebugBoundary = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary Settings")
	bool bDebugEdgeTreshhold = false;

	UFUNCTION(BlueprintCallable, Category = "Boid Manager Settings", meta = (DisplayName = "Set Boid Manager Properties Struct", ToolTip = "Sets all boid manager properties using a single struct."))
	void SetProperties(const FBoidManagerProperties& NewProperties);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Boid Manager Settings", meta = (DisplayName = "Get Boid Manager Properties Struct", ToolTip = "Gets all boid manager properties using a single struct."))
	FBoidManagerProperties GetProperties();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<class AFlockingBoid*> GetBoidNeighbourhood(class AFlockingBoid* ThisBoid);
	FVector GetClosestBoidPosition(class AFlockingBoid* ThisBoid, bool bDebugPrint = false);

};



