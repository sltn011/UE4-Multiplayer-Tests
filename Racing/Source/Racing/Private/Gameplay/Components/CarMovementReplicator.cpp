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
		SimProxy_Tick(DeltaTime);
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
	ENetRole Role = GetOwnerRole();
	switch (GetOwnerRole()) {
	case ROLE_AutonomousProxy:
		AutoProxy_OnRepl_ServerState();
		break;

	case ROLE_SimulatedProxy:
		SimProxy_OnRepl_ServerState();
		break;
	}
}

void UCarMovementReplicator::AutoProxy_OnRepl_ServerState(
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

void UCarMovementReplicator::SimProxy_OnRepl_ServerState(
) {
	if (!GetOwner() || !MovementComponent) {
		return;
	}

	Client_TimeBetweenLastUpdates = Client_TimeSinceUpdate;
	Client_TimeSinceUpdate = 0.0f;
	Client_StartTransform = GetOwner()->GetActorTransform();
	Client_StartVelocity = MovementComponent->GetVelocity();
}

void UCarMovementReplicator::SimProxy_Tick(
	float DeltaTime
) {
	Client_TimeSinceUpdate += DeltaTime;

	if (Client_TimeBetweenLastUpdates < SMALL_NUMBER) {
		return;
	}

	float LerpAlpha = Client_TimeSinceUpdate / Client_TimeBetweenLastUpdates;
	float VelocityToDerivative = Client_TimeBetweenLastUpdates * 100;

	FVector StartDerivative = Client_StartVelocity * VelocityToDerivative;
	FVector TargetDerivative = ServerState.Velocity * VelocityToDerivative;

	FVector NewLocation = FMath::CubicInterp(
		Client_StartTransform.GetLocation(),
		StartDerivative,
		ServerState.Transform.GetLocation(),
		TargetDerivative,
		LerpAlpha
	);

	FVector NewDerivative = FMath::CubicInterpDerivative(
		Client_StartTransform.GetLocation(),
		StartDerivative,
		ServerState.Transform.GetLocation(),
		TargetDerivative,
		LerpAlpha
	);
	FVector NewVelocity = NewDerivative / VelocityToDerivative;

	FQuat NewRotation = FQuat::Slerp(
		Client_StartTransform.GetRotation(),
		ServerState.Transform.GetRotation(),
		LerpAlpha
	);
	
	AActor *OwningPawn = GetOwner();
	if (!OwningPawn) {
		return;
	}
	OwningPawn->SetActorLocation(NewLocation);
	MovementComponent->SetVelocity(NewVelocity);
	OwningPawn->SetActorRotation(NewRotation);
}

// Setup property replication to receive updates for replicated values from server
void UCarMovementReplicator::GetLifetimeReplicatedProps(
	TArray< FLifetimeProperty > &OutLifetimeProps
) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCarMovementReplicator, ServerState);
}
