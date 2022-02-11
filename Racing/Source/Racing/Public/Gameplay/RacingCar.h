// Multiplayer game with Cars

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RacingCar.generated.h"

class UBoxComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USpringArmComponent;

UCLASS()
class RACING_API ARacingCar : public APawn
{
	GENERATED_BODY()

public:

	ARacingCar(
	);

protected:

	virtual void BeginPlay(
	) override;

	virtual void MoveForward(
		float Value
	);

	virtual void Turn(
		float Value
	);

	void UpdateLocationWithVelocity(
		float DeltaTime
	);

	void UpdateRotation(
		float DeltaTime
	);

	FVector GetResistance(
	);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent *BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent *Camera;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.0f))
	float CarMass = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.0f))
	float MaxDrivingForce = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.0f))
	float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.0f))
	float DragCoefficient = 15.0f;

	float Throttle = 0.0f;
	FVector Velocity = FVector::ZeroVector;
	float RotationValue = 0.0f;

public:	

	virtual void Tick(
		float DeltaTime
	) override;

	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent
	) override;

};
