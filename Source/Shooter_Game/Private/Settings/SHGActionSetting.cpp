// Shooter_Game, All rights reserved.

#include "Settings/SHGActionSetting.h"

void USHGActionSetting::DoAction()
{
    if (!Action)
        return;

    Action();
}

bool USHGActionSetting::IsActionAvailable()
{
    if (!Status)
        return false;

    return Status();
}
