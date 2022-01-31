// Shooter_Game, All rights reserved.

#include "UI/ShooterHUDUserWidget.h"

bool UShooterHUDUserWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}