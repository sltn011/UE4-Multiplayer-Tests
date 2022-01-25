// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class PLATFORMER_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	

	AMovingPlatform();

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (MakeEditWidget = true))
	FVector TargetPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = 0.0f))
	float MoveSpeed;

	virtual void BeginPlay() override;

private:	

	FVector StartPoint;
	FVector GlobalTargetPoint;

	float Distance = 0.0f;

	float MovementCounter = 0.0f;
	bool bForwardMoving = true;
};
