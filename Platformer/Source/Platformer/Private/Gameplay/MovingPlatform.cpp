// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	StartPoint = GetActorLocation();
	GlobalTargetPoint = GetTransform().TransformPosition(TargetPoint);
	Distance = FVector{ GlobalTargetPoint - StartPoint }.Size();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Prevent platform from moving on client
	// Only moves on client-server
	if (!HasAuthority()) {
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector DestinationLocation = GlobalTargetPoint;

	FVector Direction = DestinationLocation - StartPoint;
	Direction = bForwardMoving ? Direction : -Direction;
	Direction = Direction.GetSafeNormal();

	float Offset = MoveSpeed * DeltaTime;
	FVector MoveDelta = Direction * Offset;
	FVector NewLocation = CurrentLocation + MoveDelta;

	MovementCounter += Offset;
	if (MovementCounter > Distance) {
		MovementCounter = 0.0f;
		bForwardMoving ^= true;
	}

	SetActorLocation(NewLocation);
}

