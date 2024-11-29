// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BoidFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BOIDS_API UBoidFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		meta = (
			DisplayName = "Calculate seek direction",
			Keywords = "Seek, Direction"),
		Category = "BoidLibrary|Math"
	)
	static FVector Seek(FVector ActorPosition, FVector TargetPosition);

	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		meta = (
			DisplayName = "Calculate flee direction",
			Keywords = "Flee, Direction"),
		Category = "BoidLibrary|Math"
	)
	static FVector Flee(FVector ActorPosition, FVector TargetPosition);
};
