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
	ACoopCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	class UCoopHealthComponent* HealthComponent;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	int32 CollectedItems = 0;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "UI")
	bool bIsDangerNear = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
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
