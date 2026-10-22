#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopHazardActor.generated.h"

class ACoopCharacter;

UCLASS()
class COOPESCAPE_API ACoopHazardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoopHazardActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	class UCoopHazardConfig* Config;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	UMaterialInterface* ActiveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	UMaterialInterface* InactiveMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_IsActive, BlueprintReadOnly, Category = "Hazard")
	bool bIsActive = false;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void MoveTowardsPlayer(AActor* Player, float DeltaTime);
	void ReturnToBase(float DeltaTime);
	void ApplyDamageToPlayer(AActor* Player);
	void ResetDamageCooldown();
	void UpdateVisualState();

	AActor* FindClosestPlayer() const;

	UFUNCTION()
	void OnRep_IsActive();

	UFUNCTION(Server, Reliable)
	void Server_UpdateDangerFlag(ACoopCharacter* Player, bool bDanger);

private:
	FVector InitialLocation;
	bool bCanApplyDamage = true;
	FTimerHandle DamageCooldownTimer;

	UPROPERTY()
	ACoopCharacter* CurrentTarget = nullptr;
	
	void UpdateHazardState(AActor* ClosestPlayer, float Distance);
};
