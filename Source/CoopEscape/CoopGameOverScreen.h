// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoopGameOverScreen.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class COOPESCAPE_API UCoopGameOverScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetResult(bool bWon);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Restart;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Exit;

	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnExitClicked();
};
