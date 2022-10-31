// Shooter_Game, All rights reserved.

#include "Settings/SHGIntSetting.h"

int32 USHGIntSetting::GetCurrentValue() const
{
    if (!Getter)
        return INDEX_NONE;

    return Getter();
}

void USHGIntSetting::SetValue(int32 NewValue)
{
    if (!Setter)
        return;

    Setter(NewValue);
}
