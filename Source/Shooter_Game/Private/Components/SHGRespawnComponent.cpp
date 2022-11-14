// Shooter_Game, All rights reserved.

#include "Components/SHGRespawnComponent.h"
#include "SHGGameModeArena.h"

USHGRespawnComponent::USHGRespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGRespawnComponent::StartRespawn(float RespawnTime)
{
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::OnEndRespawn, RespawnTime);
}

bool USHGRespawnComponent::GetCurrentRespawnTime(float& CurrentRespawnTime) const
{
    if (GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle))
    {
        CurrentRespawnTime = GetWorld()->GetTimerManager().GetTimerRemaining(RespawnTimerHandle);
        return true;
    }

    return false;
}

void USHGRespawnComponent::OnEndRespawn()
{
    const auto Controller = GetOwner<AController>();
    const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeArena>();
    if (!Controller || !GameMode)
        return;

    GameMode->RespawnRequest(Controller);
}
