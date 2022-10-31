// Shooter_Game, All rights reserved.

#include "UI/Gameplay/SHGImpactIndicatorUserWidget.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"

void USHGImpactIndicatorUserWidget::SetImpactWidgetText(const FText& NewText)
{
    ImpactText->SetText(NewText);
}

void USHGImpactIndicatorUserWidget::SetImpactWidgetTextColor(const FLinearColor& NewColor)
{
    ImpactText->SetColorAndOpacity(FSlateColor(NewColor));
}

void USHGImpactIndicatorUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ImpactText);
}
