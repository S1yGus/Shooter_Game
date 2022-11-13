// Shooter_Game, All rights reserved.

#include "Components/ShooterRespawnComponent.h"
#include "SHGGameModeArena.h"

USHGRespawnComponent::USHGRespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGRespawnComponent::StartRespawn(int32 RespawnTime)
{
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::EndRespawn, RespawnTime);
}

bool USHGRespawnComponent::GetCurrentRespawnTime(float& CurrentRespawnTime) const
{
    if (!GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle))
        return false;

    CurrentRespawnTime = GetWorld()->GetTimerManager().GetTimerRemaining(RespawnTimerHandle);
    return true;
}

void USHGRespawnComponent::EndRespawn()
{
    const auto GameMode = Cast<ASHGGameModeArena>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
        return;

    const auto Controller = Cast<AController>(GetOwner());
    if (!Controller)
        return;

    GameMode->RespawnRequest(Controller);
}

void USHGRespawnComponent::BeginPlay()
{
    Super::BeginPlay();
}
