// Multiplayer game with Cars


#include "Gameplay/RacingCar.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRacingCar, All, All);

ARacingCar::ARacingCar(
) {
 	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector{ 230.0f, 100.0f, 65.0f });
	SetRootComponent(BoxCollision);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->AddRelativeLocation(FVector{ -5.0f, 0.0f, -85.0f });
	Mesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AddRelativeLocation(FVector{ -210.0f, 0.0f, 155.0f });
	SpringArm->AddRelativeRotation(FRotator{ -20.0f, 0.0f, 0.0f });
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

void ARacingCar::Tick(
	float DeltaTime
) {
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	FVector Acceleration = Force / CarMass;

	Velocity += Acceleration * DeltaTime;

	FVector Translation = Velocity * 100 * DeltaTime;

	FHitResult HitResult;
	AddActorWorldOffset(Translation, true, &HitResult);
	if (HitResult.IsValidBlockingHit()) {
		Velocity = FVector::ZeroVector;
	}
}

void ARacingCar::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent
) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent) {
		PlayerInputComponent->BindAxis("MoveForward", this, &ARacingCar::MoveForward);
	}
}

