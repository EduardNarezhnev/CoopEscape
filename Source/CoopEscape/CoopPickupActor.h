#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopPickupActor.generated.h"

UCLASS()
class COOPESCAPE_API ACoopPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoopPickupActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* PickupSound;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin( 
		UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

	virtual void ApplyEffect(class ACoopCharacter* Character);
};
