#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CoopExtractionTrigger.generated.h"

UCLASS()
class COOPESCAPE_API ACoopExtractionTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	ACoopExtractionTrigger();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	bool AreAllPlayersInZone() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;
};
