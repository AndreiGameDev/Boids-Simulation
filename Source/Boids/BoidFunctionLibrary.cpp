// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidFunctionLibrary.h"
#include "Logging/StructuredLog.h"

FVector UBoidFunctionLibrary::Seek(FVector ActorPosition, FVector TargetPosition)
{
	FVector NewVelocity = TargetPosition - ActorPosition;
	NewVelocity.Normalize();
	return NewVelocity;
}


FVector UBoidFunctionLibrary::Flee(FVector ActorPosition, FVector TargetPosition)
{
	FVector NewVelocity = ActorPosition - TargetPosition;
	NewVelocity.Normalize();
	return NewVelocity;
}


