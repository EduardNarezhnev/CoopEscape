// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CoopGameState.generated.h"

/**
 * 
 */
UCLASS()
class COOPESCAPE_API ACoopGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game")
	int32 CollectedItemsCount = 0;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
