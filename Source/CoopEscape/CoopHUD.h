// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoopHUD.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class COOPESCAPE_API UCoopHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHUD();

	void ShowNotification(const FString& Message, float Duration = 2.f);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PersonalKeysText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TotalKeysText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DangerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NotificationText;

	FTimerHandle NotificationTimer;

private:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	FLinearColor DangerColor = FLinearColor::Red;
};
