// Shooter_Game, All rights reserved.

#include "UI/ShooterSpectatingUserWidget.h"
#include "Components/ShooterRespawnComponent.h"

bool UShooterSpectatingUserWidget::GetCurrentRespawnTime(float& RespawnTime) const
{
    if (!GetOwningPlayer())
        return false;

    const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<UShooterRespawnComponent>();
    if (!RespawnComponent)
        return false;

    return RespawnComponent->GetCurrentRespawnTime(RespawnTime);
}