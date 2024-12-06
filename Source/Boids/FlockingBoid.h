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

public:	
	// Sets default values for this actor's properties
	AFlockingBoid();

	AFlockingBoidManager* FlockingBoidManager;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector Seek(FVector Position);
	FVector Flee(FVector Position);

	FVector Wander(float Radius, float Distance, float Jitter);
	FVector WanderDestination;

	FVector Alignment(TArray<AFlockingBoid*> Neighbours);
	FVector Cohesion(TArray<AFlockingBoid*> Neighbours);
	FVector Sepparation(TArray<AFlockingBoid*> Neighbours);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Settings")
	float Speed = 100.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateBoid(float DeltaTime);
};
