// Multiplayer game with Cars


#include "Gameplay/RacingCar.h"

// Common
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/Components/CarMovementComponent.h"
#include "Gameplay/Components/CarMovementReplicator.h"

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

	Movement = CreateDefaultSubobject<UCarMovementComponent>(TEXT("Movement"));

	MovementReplicator = CreateDefaultSubobject<UCarMovementReplicator>(TEXT("MovementReplicator"));

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

	SetReplicateMovement(false);

	if (HasAuthority()) {
		NetUpdateFrequency = 1.0f;
	}
}

void ARacingCar::Tick(
	float DeltaTime
) {
	Super::Tick(DeltaTime);

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
		PlayerInputComponent->BindAxis("MoveForward", Movement, &UCarMovementComponent::SetThrottle);
		PlayerInputComponent->BindAxis("MoveRight", Movement, &UCarMovementComponent::SetRotationDirection);
	}
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
