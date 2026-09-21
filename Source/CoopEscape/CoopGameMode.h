// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoopGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COOPESCAPE_API ACoopGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACoopGameMode();

protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;
};
