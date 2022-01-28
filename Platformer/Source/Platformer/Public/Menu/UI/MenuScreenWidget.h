// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/MenuBaseWidget.h"
#include "MenuScreenWidget.generated.h"

class UButton;
class UEditableText;

UCLASS()
class PLATFORMER_API UMenuScreenWidget : public UMenuBaseWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize(
	) override;
	
protected:

	UPROPERTY(Meta = (BindWidget))
	UButton *HostButton;

	UPROPERTY(Meta = (BindWidget))
	UButton *JoinButton;

	UPROPERTY(Meta = (BindWidget))
	UEditableText *ServerIPInput;

	UFUNCTION()
	virtual void OnHostButtonClicked(
	);

	UFUNCTION()
	virtual void OnJoinButtonClicked(
	);
};
