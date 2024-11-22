// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class BOIDS_API ABoid : public AActor
{
	GENERATED_BODY()
	
	FVector CurrentVelocity = FVector::ZeroVector;

public:	
	// Sets default values for this actor's properties
	ABoid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector Seek(FVector Position);
	FVector Flee(FVector Position);
	FVector ClosestBoidPosition(ABoid* ThisBoid);

	UStaticMeshComponent* Mesh;
	float Speed = 10;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateBoid(float DeltaTime);
};
