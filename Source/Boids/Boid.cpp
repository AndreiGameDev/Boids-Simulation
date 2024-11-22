// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "BoidManager.h"

// Sets default values
ABoid::ABoid()
{
	FVector TargetVelocity = FVector::ZeroVector;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	UStaticMesh* SphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh '/Engine/BasicShapes/Sphere.Sphere'")).Object;

	Mesh->SetStaticMesh(SphereMesh);
	this->SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ABoid::Seek(FVector Position)
{
	FVector NewVelocity = Position - GetActorLocation();
	NewVelocity.Normalize();
	return FVector();
}

FVector ABoid::Flee(FVector Position)
{
	FVector NewVelocity = GetActorLocation() - Position;
	NewVelocity.Normalize();
	return FVector();
}

FVector ABoid::ClosestBoidPosition(ABoid* ThisBoid)
{
	float ClosestDistannce = ABoidManager::MaxFleeDistance;
	FVector ReturnVal = FVector::ZeroVector;

	for (ABoid* Boid : ABoidManager::MyBoids) {
		if (Boid == this || !Boid || Boid == ABoidManager::LastTagged) {
			continue;
		}
		float ADistance = (Boid->GetActorLocation() - ThisBoid->GetActorLocation()).Size();
		if (ADistance < ClosestDistannce) {
			ClosestDistannce = ADistance;
			ReturnVal = Boid->GetActorLocation();
		}
	}
	return FVector();
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoid::UpdateBoid(float DeltaTime)
{
	FVector TargetVelocity = FVector::ZeroVector;

	// Find Velocity

	FVector NewForce = TargetVelocity - CurrentVelocity;
	CurrentVelocity += NewForce + DeltaTime;

	FVector Location = GetActorLocation();
	Location += (CurrentVelocity * Speed * DeltaTime);

	SetActorLocation(Location);
}

