// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

class ATriggerablePlatform;
class UBoxComponent;

UCLASS()
class PLATFORMER_API APlatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:	

	APlatformTrigger(
	);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent *TriggerVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TArray<ATriggerablePlatform *> TriggerablePlatforms;

	virtual void BeginPlay(
	) override;

	UFUNCTION()
	void OnPushed(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex, 
		bool bFromSweep,
		const FHitResult &SweepResult
	);

	UFUNCTION()
	void OnReleased(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex
	);

};
