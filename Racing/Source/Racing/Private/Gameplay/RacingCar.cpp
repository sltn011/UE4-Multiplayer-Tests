// Multiplayer game with Cars


#include "Gameplay/RacingCar.h"

// Common
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
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

	if (HasAuthority()) {
		NetUpdateFrequency = 1.0f;
	}
}

void ARacingCar::MoveForward(
	float Value
) {
	Throttle = Value;
}

void ARacingCar::MoveRight(
	float Value
) {
	RotationDirection = Value * FMath::Sign(FVector::DotProduct(Velocity, GetActorForwardVector()));
}

void ARacingCar::Server_SendMove_Implementation(
	FRacingCarMove const &CarMovement
) {
	SimulateMove(CarMovement);

	ServerState.Transform = GetActorTransform();
	ServerState.Velocity = Velocity;
	ServerState.LastMove = CarMovement;
}

bool ARacingCar::Server_SendMove_Validate(
	FRacingCarMove const &CarMovement
) {
	return true; // TODO: Make better validation
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
	float DeltaTime,
	float RotDirection
) {
	// dx = dTheta * r

	float DeltaLocation = Velocity.Size() * DeltaTime;
	float RotationAngle = (DeltaLocation / TurningCircleRadius) * RotDirection; // in radians
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

void ARacingCar::SimulateMove(
	FRacingCarMove const &CarMovement
) {
	FVector Force = GetActorForwardVector() * MaxDrivingForce * CarMovement.Throttle;
	Force -= GetResistance();
	FVector Acceleration = Force / CarMass;

	Velocity += Acceleration * CarMovement.DeltaTime;

	UpdateLocationWithVelocity(CarMovement.DeltaTime);
	UpdateRotation(CarMovement.DeltaTime, CarMovement.RotationDirection);
}

FRacingCarMove ARacingCar::CreateCarMove(
	float DeltaTime
) const {
	FRacingCarMove CarMovement;
	CarMovement.Throttle = Throttle;
	CarMovement.RotationDirection = RotationDirection;
	CarMovement.DeltaTime = DeltaTime;
	CarMovement.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	
	return CarMovement;
}

void ARacingCar::ClearAcknowledgedMoves(
	FRacingCarMove const &LastMove
) {
	TArray<FRacingCarMove> NewMoves;

	for (FRacingCarMove const &Move : UnacknowledgedMoves) {
		if (Move.Time > LastMove.Time) {
			NewMoves.Add(Move);
		}
	}

	UnacknowledgedMoves = MoveTemp(NewMoves);
}

void ARacingCar::OnRepl_ServerState(
) {
	SetActorTransform(ServerState.Transform);
	Velocity = ServerState.Velocity;

	ClearAcknowledgedMoves(ServerState.LastMove);

	for (FRacingCarMove const &Move : UnacknowledgedMoves) {
		SimulateMove(Move);
	}
}

void ARacingCar::Tick(
	float DeltaTime
) {
	Super::Tick(DeltaTime);

	ENetRole PawnRole = GetLocalRole();
	if (PawnRole == ROLE_AutonomousProxy) { // Client
		FRacingCarMove CarMovement = CreateCarMove(DeltaTime);
		UnacknowledgedMoves.Add(CarMovement);
		SimulateMove(CarMovement);
		Server_SendMove(CarMovement);
	}
	else if (PawnRole == ROLE_Authority && IsLocallyControlled()) { // Server-Client
		FRacingCarMove CarMovement = CreateCarMove(DeltaTime);
		Server_SendMove(CarMovement);
	}

	if (PawnRole == ROLE_SimulatedProxy) {
		SimulateMove(ServerState.LastMove);
	}

	// Debug helper
	DrawDebugString(
		GetWorld(),
		FVector{ 0.0f, 0.0f, 100.0f },
		GetRoleString(),
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
	DOREPLIFETIME(ARacingCar, ServerState);
}

FString ARacingCar::GetRoleString(
) const {
	switch (GetLocalRole()) {
	case ROLE_Authority:
		return TEXT("Authority");

	case ROLE_AutonomousProxy:
		return TEXT("AutonomousProxy");

	case ROLE_SimulatedProxy:
		return TEXT("SimulatedProxy");

	default:
		return TEXT("UnknownRole");
	}
}
