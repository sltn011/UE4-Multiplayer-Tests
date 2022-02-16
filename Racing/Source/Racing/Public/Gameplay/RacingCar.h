// Multiplayer game with Cars

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RacingCoreTypes.h"
#include "RacingCar.generated.h"

class UBoxComponent;
class UCameraComponent;
class UCarMovementComponent;
class UCarMovementReplicator;
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

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent *BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCarMovementComponent *Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCarMovementReplicator *MovementReplicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent *Camera;

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
