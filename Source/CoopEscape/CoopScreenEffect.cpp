// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopScreenEffect.h"
#include "Components/Image.h"
#include "TimerManager.h"

void UCoopScreenEffect::NativeConstruct()
{
    Super::NativeConstruct();

    if(FlashImage)
    {
        FlashImage->SetVisibility(ESlateVisibility::Hidden);
        FlashImage->SetColorAndOpacity(FLinearColor::Transparent);
    }
}

void UCoopScreenEffect::PlayEffect(const FLinearColor& Color, float Duration)
{
   if (!FlashImage) return;

   CurrentColor = Color;
   CurrentAlpha = 1.f;
   FadeSpeed = 1.f / FMath::Max(Duration, 0.01f);

   FlashImage->SetVisibility(ESlateVisibility::Visible);
   FlashImage->SetColorAndOpacity(CurrentColor);

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(EffectTimer);
        World->GetTimerManager().SetTimer(
            EffectTimer,
            this,
            &UCoopScreenEffect::UpdateEffect,
            UpdateInterval,
            true
        );
    }
}

void UCoopScreenEffect::UpdateEffect()
{
    if(!FlashImage) return;

    CurrentAlpha = FMath::Max(0.f, CurrentAlpha - FadeSpeed * UpdateInterval);
    FLinearColor NewColor = CurrentColor;
    NewColor.A = CurrentAlpha;
    FlashImage->SetColorAndOpacity(NewColor);

    if(CurrentAlpha <= 0.f)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(EffectTimer);
        }
        FlashImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

