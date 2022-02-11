// Multiplayer game with Cars


#include "Gameplay/RacingCar.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

ARacingCar::ARacingCar(
) {
 	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AddRelativeLocation(FVector{ -175.0f, 0.0f, 165.0f });
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
	//AddMovementInput(GetActorForwardVector(), Value * CarSpeed);
	Velocity = GetActorForwardVector() * Value * CarSpeed;
}

void ARacingCar::Tick(
	float DeltaTime
) {
	Super::Tick(DeltaTime);

	AddActorWorldOffset(Velocity * DeltaTime);
}

void ARacingCar::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent
) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARacingCar::MoveForward);
}

