// Multiplayer game with Cars


#include "Gameplay/RacingCar.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRacingCar, All, All);

ARacingCar::ARacingCar(
) {
 	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector{ 230.0f, 100.0f, 120.0f });
	SetRootComponent(BoxCollision);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetRelativeLocation(FVector{ -5.0f, 0.0f, -150.0f });
	Mesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector{ -210.0f, 0.0f, 155.0f });
	SpringArm->SetRelativeRotation(FRotator{ -20.0f, 0.0f, 0.0f });
	SpringArm->SetupAttachment(Mesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, SpringArm->SocketName);
}

void ARacingCar::BeginPlay(
) {
	Super::BeginPlay();
}

void ARacingCar::MoveForward(
	float Value
) {
	Throttle = Value;
}

void ARacingCar::Turn(
	float Value
) {
	RotationDirection = Value * FMath::Sign(FVector::DotProduct(Velocity, GetActorForwardVector()));
}

void ARacingCar::UpdateLocationWithVelocity(
	float DeltaTime
) {
	FVector Translation = Velocity * 100 * DeltaTime;

	FHitResult HitResult;
	AddActorWorldOffset(Translation, true, &HitResult);
	if (HitResult.IsValidBlockingHit()) {
		Velocity = FVector::ZeroVector;
	}
}

void ARacingCar::UpdateRotation(
	float DeltaTime
) {
	// dx = dTheta * r

	float DeltaLocation = Velocity.Size() * DeltaTime;
	float RotationAngle = (DeltaLocation / TurningCircleRadius) * RotationDirection; // in radians
	FQuat RotationQuat{ GetActorUpVector(), RotationAngle };

	Velocity = RotationQuat * Velocity;

	AddActorWorldRotation(RotationQuat);
}

FVector ARacingCar::GetResistance(
) {
	return GetAirResistance() + GetRollingResistance();
}

FVector ARacingCar::GetAirResistance(
) {
	return Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector ARacingCar::GetRollingResistance(
) {
	UWorld *World = GetWorld();
	float GravityAcceleration = World ? -World->GetGravityZ() / 100.0f : 9.81f;
	float WeightForce = CarMass * GravityAcceleration;
	return Velocity.GetSafeNormal() * WeightForce * RollResistCoefficient;
}

void ARacingCar::Tick(
	float DeltaTime
) {
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	Force -= GetResistance();
	FVector Acceleration = Force / CarMass;

	Velocity += Acceleration * DeltaTime;

	UpdateLocationWithVelocity(DeltaTime);
	UpdateRotation(DeltaTime);
}

void ARacingCar::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent
) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent) {
		PlayerInputComponent->BindAxis("MoveForward", this, &ARacingCar::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ARacingCar::Turn);
	}
}

