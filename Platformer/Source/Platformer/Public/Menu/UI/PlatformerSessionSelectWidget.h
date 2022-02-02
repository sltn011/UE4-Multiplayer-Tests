// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/MenuBaseWidget.h"
#include "PlatformerSessionSelectWidget.generated.h"

class UButton;

UCLASS()
class PLATFORMER_API UPlatformerSessionSelectWidget : public UMenuBaseWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize(
	) override;

	void SetSessionIDString(
		FString const &NewSessionIDString
	);

	UFUNCTION(BlueprintCallable)
	bool GetSessionIDString(
		FString &Result
	) const;

protected:

	UFUNCTION()
	void OnSessionButtonClick(
	);

	UPROPERTY(meta = (BindWidget))
	UButton *SessionButton;

	FString SessionIDString;
};
