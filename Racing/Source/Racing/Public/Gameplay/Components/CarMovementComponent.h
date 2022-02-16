// Multiplayer game with Cars

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RacingCoreTypes.h"
#include "CarMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACING_API UCarMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCarMovementComponent(
	);

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction *ThisTickFunction
	) override;

	void SimulateMove(
		FRacingCarMove const &CarMovement
	);

	FRacingCarMove GetLastMove(
	) const;

	FVector GetVelocity(
	) const;

	void SetVelocity(
		FVector Val
	);

	void SetThrottle(
		float Val
	);

	void SetRotationDirection(
		float Val
	);

protected:

	virtual void BeginPlay(
	) override;

	FRacingCarMove CreateCarMove(
		float DeltaTime
	) const;

	void UpdateLocationWithVelocity(
		float DeltaTime
	);

	void UpdateRotation(
		float DeltaTime,
		float RotDirection
	);

	FVector GetResistance(
	) const;

	FVector GetAirResistance(
	) const;

	FVector GetRollingResistance(
	) const;

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

	FRacingCarMove LastMove;
};
