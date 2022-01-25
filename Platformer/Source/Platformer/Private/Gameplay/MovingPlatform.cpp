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

	_StartPoint = GetActorLocation();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Prevent platform from moving on client
	// Only moves on client-server
	//if (!HasAuthority()) {
	//	return;
	//}

	FVector CurrentLocation = GetActorLocation();
	FVector Destination = _StartPoint + DestinationOffset;

	FVector Direction = DestinationOffset;
	Direction = _bForwardMoving ? Direction : -Direction;
	Direction = Direction.GetSafeNormal();

	FVector MoveDelta = Direction * MoveSpeed * DeltaTime;
	FVector NewLocation = CurrentLocation + MoveDelta;

	float Bias = 1.0f;
	if (FVector::PointsAreNear(NewLocation, Destination, Bias)) {
		_bForwardMoving = false;
	}
	else if (FVector::PointsAreNear(NewLocation, _StartPoint, Bias)) {
		_bForwardMoving = true;
	}

	SetActorLocation(NewLocation);
}

