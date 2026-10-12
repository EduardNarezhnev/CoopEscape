// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CoopPlayerController.generated.h"

class UCoopHUD;
class UCoopMainMenu;
class UCoopGameOverScreen;

UCLASS()
class COOPESCAPE_API ACoopPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UCoopHUD* HUDWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UCoopMainMenu* MainMenuWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UCoopGameOverScreen* GameOverWidget = nullptr;

	UFUNCTION(Client, Reliable)
	void Client_ShowGameOver(bool bWon);

	UFUNCTION(Server, Reliable)
	void Server_RestartGame();

protected:
    void ShowMainMenu();
    void ShowHUD();
};
