// Shooter_Game, All rights reserved.

#pragma once

#include "SHGSettingWidgetInterface.generated.h"

UINTERFACE(MinimalAPI)
class USHGSettingWidgetInterface : public UInterface
{
    GENERATED_BODY()
};

class ISHGSettingWidgetInterface
{
    GENERATED_BODY()

public:
    virtual void Update() = 0;
};
