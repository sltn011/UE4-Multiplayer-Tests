// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/MenuBaseWidget.h"
#include "PlatformerSessionSelectWidget.generated.h"

class UButton;
class FOnlineSession;

USTRUCT(BlueprintType)
struct FSessionData 
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FString IDString;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FString HostName;

	UPROPERTY(BlueprintReadOnly)
	int32 OpenPlayerSlots;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;

	int32 SessionIndex;
};

UCLASS()
class PLATFORMER_API UPlatformerSessionSelectWidget : public UMenuBaseWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize(
	) override;

	void FillSessionData(
		FOnlineSession const &Session,
		int32 SessionIndex
	);

	UFUNCTION(BlueprintCallable)
	bool GetSessionData(
		FSessionData &Result
	) const;

protected:

	UFUNCTION()
	void OnSessionButtonClick(
	);

	UPROPERTY(meta = (BindWidget))
	UButton *SessionButton;

	FSessionData SessionData;
};
