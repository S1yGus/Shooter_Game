// Shooter_Game, All rights reserved.

#include "UI/ShooterHealthBarUserWidget.h"
#include "Components/ProgressBar.h"

void UShooterHealthBarUserWidget::SetHealthPercent(float NewHealthPercent)
{
    if (!HealthProgressBar)
        return;

    HealthProgressBar->SetPercent(NewHealthPercent);
    HealthProgressBar->SetVisibility(NewHealthPercent > HealthPercentVisibilityThreshold || FMath::IsNearlyZero(NewHealthPercent) ? ESlateVisibility::Hidden
                                                                                                                                  : ESlateVisibility::Visible);
    HealthProgressBar->SetFillColorAndOpacity(NewHealthPercent < HealthColorThreshold ? LowHealthColor : NormalHealthColor);
}