// Multiplayer game with Cars

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RacingCar.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent *Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.0f))
	float CarSpeed;

	FVector Velocity;

public:	

	virtual void Tick(
		float DeltaTime
	) override;

	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent
	) override;

};
