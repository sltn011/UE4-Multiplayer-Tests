// Multiplayer game with Cars

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RacingCar.generated.h"

class UBoxComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USpringArmComponent;

USTRUCT()
struct FRacingCarMove 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle = 0.0f;

	UPROPERTY()
	float RotationDirection = 0.0f;

	UPROPERTY()
	float DeltaTime = 0.0f;

	UPROPERTY()
	float Time = 0.0f;

};

USTRUCT()
struct FRacingCarState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FRacingCarMove LastMove;
};

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

	void UpdateLocationWithVelocity(
		float DeltaTime
	);

	void UpdateRotation(
		float DeltaTime,
		float RotDirection
	);

	FVector GetResistance(
	);

	FVector GetAirResistance(
	);

	FVector GetRollingResistance(
	);

	void SimulateMove(
		FRacingCarMove const &CarMovement
	);

	FRacingCarMove CreateCarMove(
		float DeltaTime
	) const;

	void ClearAcknowledgedMoves(
		FRacingCarMove const &LastMove
	);



	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(
		FRacingCarMove const &CarMovement
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



	FVector Velocity = FVector::ZeroVector;
	float Throttle = 0.0f;
	float RotationDirection = 0.0f;

	UPROPERTY(ReplicatedUsing = OnRepl_ServerState)
	FRacingCarState ServerState;

	UFUNCTION()
	void OnRepl_ServerState(
	);


	TArray<FRacingCarMove> UnacknowledgedMoves;

public:	

	virtual void Tick(
		float DeltaTime
	) override;

	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent
	) override;

	FString GetRoleString(
	) const;

};
