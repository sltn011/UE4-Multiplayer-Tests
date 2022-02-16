// Multiplayer game with Cars


#include "Gameplay/Components/CarMovementReplicator.h"

// Common
#include "Gameplay/Components/CarMovementComponent.h"

// Network
#include "Net/UnrealNetwork.h"

UCarMovementReplicator::UCarMovementReplicator(
) {
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
}

void UCarMovementReplicator::BeginPlay(
) {
	Super::BeginPlay();

	APawn *OwningPawn = GetOwner<APawn>();
	if (OwningPawn) {
		MovementComponent = OwningPawn->FindComponentByClass<UCarMovementComponent>();
	}
}

void UCarMovementReplicator::UpdateServerState(
	FRacingCarMove const &CarMovement
) {
	APawn *OwningPawn = GetOwner<APawn>();
	if (!MovementComponent || !OwningPawn) {
		return;
	}

	ServerState.LastMove = CarMovement;
	ServerState.Transform = OwningPawn->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

void UCarMovementReplicator::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn *OwningPawn = GetOwner<APawn>();
	if (!MovementComponent || !OwningPawn) {
		return;
	}

	FRacingCarMove LastMove = MovementComponent->GetLastMove();
	ENetRole PawnRole = OwningPawn->GetLocalRole();

	if (PawnRole == ROLE_AutonomousProxy) { // Client
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove);
	}	

	else if (PawnRole == ROLE_Authority && OwningPawn->IsLocallyControlled()) { // Server-Client
		UpdateServerState(LastMove);
	}

	if (PawnRole == ROLE_SimulatedProxy) {
		MovementComponent->SimulateMove(ServerState.LastMove);
	}
}

void UCarMovementReplicator::ClearAcknowledgedMoves(
	FRacingCarMove const &LastMove
) {
	TArray<FRacingCarMove> NewMoves;

	for (FRacingCarMove const &Move : UnacknowledgedMoves) {
		if (Move.Time > LastMove.Time) {
			NewMoves.Add(Move);
		}
	}

	UnacknowledgedMoves = MoveTemp(NewMoves);
}

void UCarMovementReplicator::Server_SendMove_Implementation(
	FRacingCarMove const &CarMovement
) {
	APawn *OwningPawn = GetOwner<APawn>();
	if (!MovementComponent || !OwningPawn) {
		return;
	}

	MovementComponent->SimulateMove(CarMovement);

	UpdateServerState(CarMovement);
}

bool UCarMovementReplicator::Server_SendMove_Validate(
	FRacingCarMove const &CarMovement
) {
	return true; // TODO: Make better validation
}

void UCarMovementReplicator::OnRepl_ServerState(
) {
	AActor *OwningPawn = GetOwner();
	if (!MovementComponent || !OwningPawn) {
		return;
	}

	OwningPawn->SetActorTransform(ServerState.Transform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgedMoves(ServerState.LastMove);

	for (FRacingCarMove const &Move : UnacknowledgedMoves) {
		MovementComponent->SimulateMove(Move);
	}
}

// Setup property replication to receive updates for replicated values from server
void UCarMovementReplicator::GetLifetimeReplicatedProps(
	TArray< FLifetimeProperty > &OutLifetimeProps
) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCarMovementReplicator, ServerState);
}
