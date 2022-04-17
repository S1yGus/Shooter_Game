// Shooter_Game, All rights reserved.

#include "UI/Impact/ShooterImpactIndicatorUserWidget.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"

void UShooterImpactIndicatorUserWidget::SetImpactWidgetText(const FText& NewText)
{
    ImpactText->SetText(NewText);
}

void UShooterImpactIndicatorUserWidget::SetImpactWidgetTextColor(const FLinearColor& NewColor)
{
    ImpactText->SetColorAndOpacity(FSlateColor(NewColor));
}
