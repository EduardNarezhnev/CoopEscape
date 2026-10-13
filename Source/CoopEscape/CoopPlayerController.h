// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CoopPlayerController.generated.h"

class UCoopHUD;
class UCoopMainMenu;
class UCoopGameOverScreen;
class UCoopScreenEffect;

UCLASS()
class COOPESCAPE_API ACoopPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ScreenEffectClass;

	UFUNCTION(Client, Reliable)
	void Client_ShowGameOver(bool bWon);

	UFUNCTION(Client, Reliable)
	void Client_ShowNotification(const FString& Message, float Duration);

	UFUNCTION(Client, Reliable)
	void Client_ShowScreenEffect(const FLinearColor& Color, float Duration);

	UFUNCTION(Server, Reliable)
	void Server_RestartGame();

protected:
    void ShowMainMenu();
    void ShowHUD();

private:
	UPROPERTY()
	UCoopHUD* HUDWidget = nullptr;

	UPROPERTY()
	UCoopMainMenu* MainMenuWidget = nullptr;

	UPROPERTY()
	UCoopGameOverScreen* GameOverWidget = nullptr;

	UPROPERTY()
	UCoopScreenEffect* ScreenEffectWidget = nullptr;

};
