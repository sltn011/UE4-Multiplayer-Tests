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

	void MoveForward(
		float Value
	);

	void MoveRight(
		float Value
	);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(
		float Value
	);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(
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

	FVector GetAirResistance(
	);

	FVector GetRollingResistance(
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
	float TurningCircleRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.0f))
	float DragCoefficient = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.0f))
	float RollResistCoefficient = 0.1f;

	float Throttle = 0.0f;
	FVector Velocity = FVector::ZeroVector;
	float RotationDirection = 0.0f;



	UPROPERTY(ReplicatedUsing = OnRepl_ReplicatedTransform)
	FTransform ReplicatedTransform;

	UFUNCTION()
	void OnRepl_ReplicatedTransform(
	);

public:	

	virtual void Tick(
		float DeltaTime
	) override;

	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent
	) override;

};
