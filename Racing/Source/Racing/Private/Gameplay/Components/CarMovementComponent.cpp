// Multiplayer game with Cars


#include "Gameplay/Components/CarMovementComponent.h"

// Common
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"

UCarMovementComponent::UCarMovementComponent(
) {
	PrimaryComponentTick.bCanEverTick = true;
}

void UCarMovementComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction *ThisTickFunction
) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn *OwningPawn = GetOwner<APawn>();

	if (!OwningPawn) {
		return;
	}
	
	if (OwningPawn->IsLocallyControlled()) {
		LastMove = CreateCarMove(DeltaTime);
		SimulateMove(LastMove);
	}
}

void UCarMovementComponent::SimulateMove(
	FRacingCarMove const &CarMovement
) {
	AActor *OwningActor = GetOwner();
	if (!OwningActor) {
		return;
	}

	FVector Force = OwningActor->GetActorForwardVector() * MaxDrivingForce * CarMovement.Throttle;
	Force -= GetResistance();
	FVector Acceleration = Force / CarMass;

	Velocity += Acceleration * CarMovement.DeltaTime;

	UpdateLocationWithVelocity(CarMovement.DeltaTime);
	UpdateRotation(CarMovement.DeltaTime, CarMovement.RotationDirection);
}

FRacingCarMove UCarMovementComponent::GetLastMove(
) const {
	return LastMove;
}

FVector UCarMovementComponent::GetVelocity(
) const {
	return Velocity;
}

void UCarMovementComponent::SetVelocity(
	FVector Val
) {
	Velocity = Val;
}

void UCarMovementComponent::SetThrottle(
	float Val
) {
	Throttle = Val;
}

void UCarMovementComponent::SetRotationDirection(
	float Val
) {
	RotationDirection = Val;
}

void UCarMovementComponent::BeginPlay(
) {
	Super::BeginPlay();
}

FRacingCarMove UCarMovementComponent::CreateCarMove(
	float DeltaTime
) const {
	FRacingCarMove CarMovement;
	CarMovement.Throttle = Throttle;
	CarMovement.RotationDirection = RotationDirection;
	CarMovement.DeltaTime = DeltaTime;
	CarMovement.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return CarMovement;
}

void UCarMovementComponent::UpdateLocationWithVelocity(
	float DeltaTime
) {
	AActor *OwningActor = GetOwner();
	if (!OwningActor) {
		return;
	}

	FVector Translation = Velocity * 100 * DeltaTime;

	FHitResult HitResult;
	OwningActor->AddActorWorldOffset(Translation, true, &HitResult);
	if (HitResult.IsValidBlockingHit()) {
		Velocity = FVector::ZeroVector;
	}
}

void UCarMovementComponent::UpdateRotation(
	float DeltaTime,
	float RotDirection
) {
	AActor *OwningActor = GetOwner();
	if (!OwningActor) {
		return;
	}

	float RotationCorrector = FMath::Sign(FVector::DotProduct(Velocity, OwningActor->GetActorForwardVector()));
	float CorrectedRotDirection = RotDirection * RotationCorrector;

	// dx = dTheta * r
	float DeltaLocation = Velocity.Size() * DeltaTime;
	float RotationAngle = (DeltaLocation / TurningCircleRadius) * CorrectedRotDirection; // in radians
	FQuat RotationQuat{ OwningActor->GetActorUpVector(), RotationAngle };

	Velocity = RotationQuat * Velocity;

	OwningActor->AddActorWorldRotation(RotationQuat);
}

FVector UCarMovementComponent::GetResistance(
) const {
	return GetAirResistance() + GetRollingResistance();
}

FVector UCarMovementComponent::GetAirResistance(
) const {
	return Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UCarMovementComponent::GetRollingResistance(
) const {
	UWorld *World = GetWorld();
	float GravityAcceleration = World ? -World->GetGravityZ() / 100.0f : 9.81f;
	float WeightForce = CarMass * GravityAcceleration;
	return Velocity.GetSafeNormal() * WeightForce * RollResistCoefficient;
}

