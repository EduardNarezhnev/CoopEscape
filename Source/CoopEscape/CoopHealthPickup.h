// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoopPickupActor.h"
#include "CoopHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class COOPESCAPE_API ACoopHealthPickup : public ACoopPickupActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float HealAmount = 20.f;

protected:
    virtual void ApplyEffect(ACoopCharacter* Character) override;
};
