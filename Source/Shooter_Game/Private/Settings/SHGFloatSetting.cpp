// Shooter_Game, All rights reserved.

#include "Settings/SHGFloatSetting.h"

float USHGFloatSetting::GetCurrentValue() const
{
    if (!Getter)
        return 0.0f;

    return Getter();
}

void USHGFloatSetting::SetValue(float NewValue)
{
    if (!Setter)
        return;

    Setter(NewValue);
}
