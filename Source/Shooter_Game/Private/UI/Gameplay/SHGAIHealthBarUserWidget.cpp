// Shooter_Game, All rights reserved.

#include "UI/Gameplay/SHGAIHealthBarUserWidget.h"
#include "Components/ProgressBar.h"

void USHGAIHealthBarUserWidget::SetHealthPercent(float NewHealthPercent)
{
    HealthProgressBar->SetPercent(NewHealthPercent);
    HealthProgressBar->SetVisibility(NewHealthPercent > HealthPercentVisibilityThreshold || FMath::IsNearlyZero(NewHealthPercent) ? ESlateVisibility::Hidden
                                                                                                                                  : ESlateVisibility::Visible);
    HealthProgressBar->SetFillColorAndOpacity(NewHealthPercent < HealthColorThreshold ? LowHealthColor : NormalHealthColor);
}

void USHGAIHealthBarUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(HealthProgressBar);
}