#include "CoopPlayerState.h"
#include "Net/UnrealNetwork.h"

void ACoopPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACoopPlayerState, PlayerCollectedItems);
}
