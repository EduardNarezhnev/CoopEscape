// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoopPickupActor.h"
#include "CoopKeyPickup.generated.h"

/**
 * 
 */
UCLASS()
class COOPESCAPE_API ACoopKeyPickup : public ACoopPickupActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 KeyCount = 1;

protected:
	virtual void ApplyEffect(ACoopCharacter* Character) override;
};
