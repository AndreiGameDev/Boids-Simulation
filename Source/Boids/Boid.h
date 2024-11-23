// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

class ABoidManager;

UCLASS()
class BOIDS_API ABoid : public AActor
{
	GENERATED_BODY()
	
	FVector CurrentVelocity = FVector::ZeroVector;

public:	
	// Sets default values for this actor's properties
	ABoid();
	ABoidManager* BoidManager;
	float WaitCounter = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector Seek(FVector Position);
	FVector Flee(FVector Position);

	UStaticMeshComponent* Mesh;
	
	float Speed = 100;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateBoid(float DeltaTime);
};
