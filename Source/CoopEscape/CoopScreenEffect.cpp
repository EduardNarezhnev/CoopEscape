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
   UE_LOG(LogTemp, Warning, TEXT("PlayEffect called! Duration = %f"), Duration);

   if (!FlashImage) 
   {
       UE_LOG(LogTemp, Warning, TEXT("FlashImage is NULL!"));
       return;
   }

   CurrentColor = Color;
   CurrentColor.A = 1.f;

   CurrentAlpha = 1.f;

   FadeSpeed = 1.f / FMath::Max(Duration, 0.01f);

   FlashImage->SetVisibility(ESlateVisibility::Visible);
   FlashImage->SetColorAndOpacity(CurrentColor);

   UE_LOG(LogTemp, Warning, TEXT("Starting timer! FadeSpeed = %f"), FadeSpeed);

   GetWorld()->GetTimerManager().ClearTimer(EffectTimer);
   GetWorld()->GetTimerManager().SetTimer(
        EffectTimer, 
        this,
        &UCoopScreenEffect::UpdateEffect,
        UpdateInterval,
        true
    );
}

void UCoopScreenEffect::UpdateEffect()
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateEffect called! CurrentAlpha = %f"), CurrentAlpha);

    if(!FlashImage) return;

    CurrentAlpha = FMath::Max(0.f, CurrentAlpha - FadeSpeed * UpdateInterval);
    UE_LOG(LogTemp, Warning, TEXT("New Alpha = %f"), CurrentAlpha);

    FLinearColor NewColor = CurrentColor;
    NewColor.A = CurrentAlpha;
    FlashImage->SetColorAndOpacity(NewColor);

    if(CurrentAlpha <= 0.f)
    {
        GetWorld()->GetTimerManager().ClearTimer(EffectTimer);
        FlashImage->SetVisibility(ESlateVisibility::Hidden);
        UE_LOG(LogTemp, Warning, TEXT("Effect finished!"));
    }
}

