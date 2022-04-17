// Shooter_Game, All rights reserved.

#include "UI/HUD/ShooterSpectatingUserWidget.h"
#include "Components/ShooterRespawnComponent.h"
#include "Kismet/KismetTextLibrary.h"

FText UShooterSpectatingUserWidget::GetCurrentRespawnTime() const
{
    if (!GetOwningPlayer())
        return FText::FromString(DeathText);

    const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<UShooterRespawnComponent>();
    if (!RespawnComponent)
        return FText::FromString(DeathText);

    float RespawnTime;
    if (!RespawnComponent->GetCurrentRespawnTime(RespawnTime))
        return FText::FromString(DeathText);

    return UKismetTextLibrary::Conv_FloatToText(RespawnTime, ERoundingMode::HalfToEven, false, true, 1, 324, 1, 1);
}

void UShooterSpectatingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (BlinkingAnimation)
    {
        PlayAnimation(BlinkingAnimation, 0.0f, 0);
    }
}
