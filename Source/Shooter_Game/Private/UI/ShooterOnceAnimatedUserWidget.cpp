// Shooter_Game, All rights reserved.

#include "UI/ShooterOnceAnimatedUserWidget.h"

void UShooterOnceAnimatedUserWidget::ShowStartupAnimation()
{
    if (CanShowStartupAnimation)
    {
        Super::ShowStartupAnimation();
        CanShowStartupAnimation = false;
    }
}
