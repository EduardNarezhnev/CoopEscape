// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "CoopCharacter.h"
#include "CoopHealthComponent.h"
#include "CoopPlayerState.h"
#include "CoopGameState.h"

void UCoopHUD::UpdateHUD()
{
    ACoopCharacter* Character = Cast<ACoopCharacter>(GetOwningPlayerPawn());
    if(!Character) return;

    if(HealthBar && Character->HealthComponent)
    {
        HealthBar->SetPercent(Character->HealthComponent->GetHealthPercent());
    }

    if(PersonalKeysText)
    {
        if(ACoopPlayerState* PS = Character->GetPlayerState<ACoopPlayerState>())
        {
            FString Text = FString::Printf(TEXT("My keys: %d"), PS->PlayerCollectedItems);
            PersonalKeysText->SetText(FText::FromString(Text));
        }
    }

    if(TotalKeysText)
    {
        if(ACoopGameState* GS = GetWorld()->GetGameState<ACoopGameState>())
        {
            FString Text = FString::Printf(TEXT("Total: %d / %d"), GS->CollectedItemsCount, GS->MaxKeys);
            TotalKeysText->SetText(FText::FromString(Text));
        }
    }

    if(DangerText)
    {
        bool bDanger = Character->bIsDangerNear;
        DangerText->SetVisibility(bDanger ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        DangerText->SetColorAndOpacity(bDanger ? DangerColor : FLinearColor::White);
    }
}

void UCoopHUD::ShowNotification(const FString& Message, float Duration)
{
    if(!NotificationText) return;

    NotificationText->SetText(FText::FromString(Message));
    NotificationText->SetVisibility(ESlateVisibility::Visible);

    GetWorld()->GetTimerManager().ClearTimer(NotificationTimer);
    GetWorld()->GetTimerManager().SetTimer(
        NotificationTimer,
        [this]()
        {
            if(NotificationText)
            {
                NotificationText->SetVisibility(ESlateVisibility::Hidden);
            }
        },
        Duration,
        false
    );
}
