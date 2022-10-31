// Shooter_Game, All rights reserved.

#include "Settings/SHGBoolSetting.h"

int32 USHGBoolSetting::GetCurrentValue() const
{
    if (!Getter)
        return INDEX_NONE;

    return static_cast<int32>(Getter());
}

void USHGBoolSetting::SetValue(bool bNewValue)
{
    if (!Setter)
        return;

    Setter(bNewValue);
}
