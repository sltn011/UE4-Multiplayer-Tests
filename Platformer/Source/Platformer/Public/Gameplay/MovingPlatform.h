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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector DestinationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 0.0f))
	float MoveSpeed;

	virtual void BeginPlay() override;

private:	

	FVector _StartPoint;

	bool _bForwardMoving = true;
};
