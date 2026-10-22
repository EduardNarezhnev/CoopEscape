#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CoopGameState.generated.h"

UCLASS()
class COOPESCAPE_API ACoopGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game")
	int32 CollectedItemsCount = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game")
	int32 MaxKeys = 3;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game")
    bool bIsPaused = false;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
