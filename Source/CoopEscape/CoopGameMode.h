// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoopGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	WaitingToStart,
	InProgress,
	GameOver
};

UCLASS()
class COOPESCAPE_API ACoopGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACoopGameMode();

	UFUNCTION(BlueprintCallable, Category = "Game")
	EGameState GetCurrentState() const { return CurrentState; }

protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

private:
	EGameState CurrentState = EGameState::WaitingToStart;
};
