#pragma once

#include "RacingCoreTypes.generated.h"

USTRUCT(BlueprintType)
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

	bool IsValid() const {
		return FMath::Abs(Throttle) <= 1.0f && FMath::Abs(RotationDirection) <= 1.0f;
	}
};

USTRUCT(BlueprintType)
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