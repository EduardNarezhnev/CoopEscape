// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoopCharacter.generated.h"

UCLASS()
class COOPESCAPE_API ACoopCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACoopCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	class UCoopHealthComponent* HealthComponent;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	int32 CollectedItems = 0;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void BP_ShowDamageEffect();

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void BP_ShowHealEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void StartJump();
    void StopJump();

private:
	UFUNCTION()
	void HandleDeath();

	UFUNCTION()
	void HandleDamage();

	UFUNCTION()
	void HandleHeal();
};
