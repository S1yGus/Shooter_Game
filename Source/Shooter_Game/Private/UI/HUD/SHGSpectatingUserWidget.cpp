// Shooter_Game, All rights reserved.

#include "UI/HUD/SHGSpectatingUserWidget.h"
#include "Components/ShooterRespawnComponent.h"
#include "Kismet/KismetTextLibrary.h"

FText USHGSpectatingUserWidget::GetCurrentRespawnTime() const
{
    const auto PC = GetOwningPlayer();
    if (!PC)
        return DeathText;

    const auto RespawnComponent = PC->FindComponentByClass<USHGRespawnComponent>();
    if (!RespawnComponent)
        return DeathText;

    float RespawnTime = 0.0f;
    if (!RespawnComponent->GetCurrentRespawnTime(RespawnTime))
        return DeathText;

    return UKismetTextLibrary::Conv_FloatToText(RespawnTime, ERoundingMode::HalfToEven, false, true, 1, 324, 1, 1);
}

void USHGSpectatingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(BlinkingAnimation);

    PlayAnimation(BlinkingAnimation, 0.0f, 0);
}
