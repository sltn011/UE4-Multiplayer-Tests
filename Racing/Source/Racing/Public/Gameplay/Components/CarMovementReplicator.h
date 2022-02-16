// Multiplayer game with Cars

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RacingCoreTypes.h"
#include "CarMovementReplicator.generated.h"

class UCarMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACING_API UCarMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCarMovementReplicator(
	);

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction *ThisTickFunction
	) override;

protected:

	virtual void BeginPlay(
	) override;

	void UpdateServerState(
		FRacingCarMove const &CarMovement
	);
		
	void ClearAcknowledgedMoves(
		FRacingCarMove const &LastMove
	);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(
		FRacingCarMove const &CarMovement
	);

	UFUNCTION()
	void OnRepl_ServerState(
	);

	UPROPERTY(ReplicatedUsing = OnRepl_ServerState)
	FRacingCarState ServerState;

	TArray<FRacingCarMove> UnacknowledgedMoves;

	UPROPERTY()
	UCarMovementComponent *MovementComponent;
};
