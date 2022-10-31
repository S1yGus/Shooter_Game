// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SHGSetting.h"
#include "SHGFloatSetting.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGFloatSetting : public USHGSetting
{
    GENERATED_BODY()

public:
    float GetCurrentValue() const;
    void SetValue(float NewValue);

private:
    TFunction<float()> Getter;
    TFunction<void(float)> Setter;

    void AddGetter(TFunction<float()> Func) { Getter = Func; }
    void AddSetter(TFunction<void(float)> Func) { Setter = Func; }

    friend class USHGGameUserSettings;
};
