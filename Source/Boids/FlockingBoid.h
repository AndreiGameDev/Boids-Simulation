// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlockingBoid.generated.h"

class AFlockingBoidManager;

UCLASS()
class BOIDS_API AFlockingBoid : public AActor
{
	GENERATED_BODY()
	
	FVector CurrentVelocity = FVector::ZeroVector;
	bool bHasNeighbourhood = false;

	
public:	
	// Sets default values for this actor's properties
	AFlockingBoid();

	AFlockingBoidManager* FlockingBoidManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SeparationValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ObjectAvoidanceRange = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ObjectAvoidanceRadius = 50;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector ApplySphereConstraints(FVector CurrentActorVelocity, FVector SphereCenter, float SphereRadius, float EdgeThreshold);
	FVector Seek(FVector Position);
	FVector CollisionAvoidance();
	TArray<FHitResult> Hits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Settings")
	float Speed = 100.0f;
private:
	void DebugNeighbourRadius();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateBoid(float DeltaTime);
};
