// Multiplayer game with Cars


#include "Gameplay/RacingCar.h"

// Common
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"

// Network
#include "Net/UnrealNetwork.h"

// Helpers
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogRacingCar, All, All);

ARacingCar::ARacingCar(
) {
 	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;

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

	//if (HasAuthority()) {
	//	NetUpdateFrequency = 1.0f;
	//}
}

void ARacingCar::MoveForward(
	float Value
) {
	Throttle = Value;
	Server_MoveForward(Value);
}

void ARacingCar::MoveRight(
	float Value
) {
	RotationDirection = Value * FMath::Sign(FVector::DotProduct(Velocity, GetActorForwardVector()));
	Server_MoveRight(Value);
}

void ARacingCar::Server_MoveForward_Implementation(
	float Value
) {
	Throttle = Value;
}

bool ARacingCar::Server_MoveForward_Validate(
	float Value
) {
	return FMath::Abs(Value) <= 1.0f;
}

void ARacingCar::Server_MoveRight_Implementation(
	float Value
) {
	RotationDirection = Value * FMath::Sign(FVector::DotProduct(Velocity, GetActorForwardVector()));
}

bool ARacingCar::Server_MoveRight_Validate(
	float Value
) {
	return FMath::Abs(Value) <= 1.0f;
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

void ARacingCar::OnRepl_ReplicatedTransform(
) {
	SetActorTransform(ReplicatedTransform);
}

void ARacingCar::Tick(
	float DeltaTime
) {
	Super::Tick(DeltaTime);

	if (HasAuthority()) {
		FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
		Force -= GetResistance();
		FVector Acceleration = Force / CarMass;

		Velocity += Acceleration * DeltaTime;

		UpdateLocationWithVelocity(DeltaTime);
		UpdateRotation(DeltaTime);

		ReplicatedTransform = GetActorTransform();
	}
	//else {
	//	SetActorLocation(ReplicatedLocation);
	//	SetActorRotation(ReplicatedRotation);
	//}

	// Debug helper
	FString RoleString;
	switch (GetLocalRole()) {
	case ROLE_Authority:
		RoleString = TEXT("Authority");
		break;

	case ROLE_AutonomousProxy:
		RoleString = TEXT("AutonomousProxy");
		break;

	case ROLE_SimulatedProxy:
		RoleString = TEXT("SimulatedProxy");
		break;

	default:
		RoleString = TEXT("UnknownRole");
		break;
	}

	DrawDebugString(
		GetWorld(),
		FVector{ 0.0f, 0.0f, 100.0f },
		RoleString,
		this,
		FColor::White,
		DeltaTime
	);
}

void ARacingCar::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent
) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent) {
		PlayerInputComponent->BindAxis("MoveForward", this, &ARacingCar::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ARacingCar::MoveRight);
	}
}

// Setup property replication to receive updates for replicated values from server
void ARacingCar::GetLifetimeReplicatedProps(
	TArray< FLifetimeProperty > &OutLifetimeProps
) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARacingCar, ReplicatedTransform);
}
