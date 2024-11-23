// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"

UCLASS()
class BOIDS_API ABoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidManager();
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int SpawnCount = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SpawnRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float MaxFleeDistance = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float ChaseMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float CatchRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float TimeoutTime = 5.0f;


	USceneComponent* Transform;

	TArray<class ABoid*> MyBoids;
	ABoid* LastTagged;
	ABoid* TaggedBoid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector ClosestBoidPosition(ABoid* ThisBoid);
};
