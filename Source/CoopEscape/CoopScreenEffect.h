// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoopScreenEffect.generated.h"

class UImage;

UCLASS()
class COOPESCAPE_API UCoopScreenEffect : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayEffect(const FLinearColor& Color, float Duration = 0.3f);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> FlashImage;

private:
	void UpdateEffect();

	FLinearColor CurrentColor = FLinearColor::Transparent;
	float CurrentAlpha = 0.f;
	float FadeSpeed = 10.f;
	FTimerHandle EffectTimer;
	const float UpdateInterval = 0.016f;
};
