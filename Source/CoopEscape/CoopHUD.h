// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoopHUD.generated.h"

/**
 * 
 */
UCLASS()
class COOPESCAPE_API UCoopHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHUD();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void BP_UpdateHealth(float HealthPercent);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void BP_UpdatePersonalKeys(int32 PersonalKeys, int32 MaxKeys);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void BP_UpdateTotalKeys(int32 TotalKeys, int32 MaxKeys);
};
