// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SHGSetting.h"
#include "SHGBoolSetting.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGBoolSetting : public USHGSetting
{
    GENERATED_BODY()

public:
    int32 GetCurrentValue() const;
    void SetValue(bool bNewValue);

private:
    TFunction<bool()> Getter;
    TFunction<void(bool)> Setter;

    void AddGetter(TFunction<bool()> Func) { Getter = Func; }
    void AddSetter(TFunction<void(bool)> Func) { Setter = Func; }

    friend class USHGGameUserSettings;
};
