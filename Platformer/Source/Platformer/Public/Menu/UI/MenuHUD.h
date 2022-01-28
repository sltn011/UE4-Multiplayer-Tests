// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

class UMenuScreenWidget;

UCLASS()
class PLATFORMER_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay(
	) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GUI")
	TSubclassOf<UMenuScreenWidget> MenuWidgetClass;

};
