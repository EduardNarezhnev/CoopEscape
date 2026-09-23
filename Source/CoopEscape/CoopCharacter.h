// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CoopCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

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

	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	void StartJump();
	void StopJump();

private:
	UFUNCTION()
	void HandleDeath();

	UFUNCTION()
	void HandleDamage(float DamageAmount);

	UFUNCTION()
	void HandleHeal(float HealAmount);
};
