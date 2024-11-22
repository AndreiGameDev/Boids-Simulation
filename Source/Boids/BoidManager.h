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


	static float MaxFleeDistance = 1000.0f;

	USceneComponent* Transform;

	static TArray<class ABoid*> MyBoids;
	static ABoid* LastTagged;
	static ABoid* TaggedBoid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	static FVector ClosestBoidPosition(ABoid* ThisBoid);
};
