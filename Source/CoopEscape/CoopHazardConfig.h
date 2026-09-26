// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CoopHazardConfig.generated.h"

/**
 * 
 */
UCLASS()
class COOPESCAPE_API UCoopHazardConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float DamageAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float ActivationDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float DeactivationDistance = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float MoveSpeed = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float DamageCooldown = 1.f;
};
