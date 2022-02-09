// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/MenuScreenWidget.h"
#include "PlatformerMenuScreenWidget.generated.h"

class FOnlineSessionSearchResult;
class UEditableText;
class UPlatformerSessionSelectWidget;
class UScrollBox;

UCLASS()
class PLATFORMER_API UPlatformerMenuScreenWidget : public UMenuScreenWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize(
	) override;

	virtual void OnHostButtonClicked(
	) override;

	virtual void OnJoinByIPButtonClicked(
	) override;

protected:

	void OnSessionsFound(
		const TArray<FOnlineSessionSearchResult> &Sessions
	);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPlatformerSessionSelectWidget> SessionSelectorClass;

	UPROPERTY(meta = (BindWidget))
	UScrollBox *AvailableSessions;

	UPROPERTY(Meta = (BindWidget))
	UEditableText *CustomSessionName;

};
