// Shooter_Game, All rights reserved.

#include "Components/ShooterRespawnComponent.h"
#include "ShooterGameModeBase.h"

UShooterRespawnComponent::UShooterRespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UShooterRespawnComponent::StartRespawn(int32 RespawnTime)
{
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UShooterRespawnComponent::EndRespawn, RespawnTime);
}

bool UShooterRespawnComponent::GetCurrentRespawnTime(float& CurrentRespawnTime) const
{
    if (!GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle))
        return false;

    CurrentRespawnTime = GetWorld()->GetTimerManager().GetTimerRemaining(RespawnTimerHandle);
    return true;
}

void UShooterRespawnComponent::EndRespawn()
{
    const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
        return;

    const auto Controller = Cast<AController>(GetOwner());
    if (!Controller)
        return;

    GameMode->RespawnRequest(Controller);
}

void UShooterRespawnComponent::BeginPlay()
{
    Super::BeginPlay();
}
