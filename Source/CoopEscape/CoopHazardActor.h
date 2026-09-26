// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopHazardActor.generated.h"

UCLASS()
class COOPESCAPE_API ACoopHazardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
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

	UFUNCTION()
	void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
	
	void MoveTowardsPlayer(AActor* Player, float DeltaTime);
	void ReturnToBase(float DeltaTime);
	void ApplyDamageToPlayer(AActor* Player);
	void ResetDamageCooldown();
	void UpdateVisualState();

	AActor* FindClosestPlayer() const;

	UFUNCTION()
	void OnRep_IsActive();

private:
	FVector InitialLocation;
	bool bCanApplyDamage = true;
	FTimerHandle DamageCooldownTimer;

	void UpdateHazardState(AActor* ClosestPlayer, float Distance);
};
