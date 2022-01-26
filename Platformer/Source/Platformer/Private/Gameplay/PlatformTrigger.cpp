// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlatformTrigger.h"

#include "Components/BoxComponent.h"
#include "Gameplay/TriggerablePlatform.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlatformTrigger, All, All);

APlatformTrigger::APlatformTrigger(
) {
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(GetRootComponent());
}

void APlatformTrigger::BeginPlay(
) {
	Super::BeginPlay();

	if (!HasAuthority()) {
		return;
	}

	check(TriggerVolume);
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnPushed);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnReleased);
}

void APlatformTrigger::OnPushed(
	UPrimitiveComponent *OverlappedComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult
) {
	UE_LOG(LogPlatformTrigger, Display, TEXT("Platform pushed by %s"), *OtherActor->GetName());

	for (ATriggerablePlatform *Platform : TriggerablePlatforms) {
		if (Platform) {
			Platform->ActivatePlatform();
		}
	}
}

void APlatformTrigger::OnReleased(
	UPrimitiveComponent *OverlappedComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex
) {
	UE_LOG(LogPlatformTrigger, Display, TEXT("Platform released by %s"), *OtherActor->GetName());

	for (ATriggerablePlatform *Platform : TriggerablePlatforms) {
		if (Platform) {
			Platform->DeactivatePlatform();
		}
	}
}
