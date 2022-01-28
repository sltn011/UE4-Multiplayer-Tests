// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/MenuScreenWidget.h"
#include "PlatformerMenuScreenWidget.generated.h"

UCLASS()
class PLATFORMER_API UPlatformerMenuScreenWidget : public UMenuScreenWidget
{
	GENERATED_BODY()
	
public:

	virtual void OnHostButtonClicked(
	);

	virtual void OnJoinButtonClicked(
	);

};
