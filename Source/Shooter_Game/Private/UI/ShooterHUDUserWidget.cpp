// Shooter_Game, All rights reserved.

#include "UI/ShooterHUDUserWidget.h"

bool UShooterHUDUserWidget::IsPlayerSpectating() const
{
    return GetOwningPlayer() && GetOwningPlayer()->GetStateName() == NAME_Spectating;
}