// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/MenuBaseWidget.h"
#include "MenuScreenWidget.generated.h"

class IMenuInterface;

class UButton;
class UEditableText;

UCLASS()
class MENUSYSTEM_API UMenuScreenWidget : public UMenuBaseWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize(
	) override;

	void SetMenuImplementation(
		IMenuInterface *MenuImplementationPtr
	);

protected:

	UPROPERTY(Meta = (BindWidget))
	UButton *HostButton;

	UPROPERTY(Meta = (BindWidget))
	UButton *JoinButton;

	UPROPERTY(Meta = (BindWidget))
	UEditableText *ServerIPInput;

	IMenuInterface *MenuImplementation = nullptr;

	UFUNCTION()
	void OnHostButtonClicked(
	);

	UFUNCTION()
	void OnJoinButtonClicked(
	);
};
