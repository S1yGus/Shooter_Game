// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SHGSetting.h"
#include "SHGIntSetting.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGIntSetting : public USHGSetting
{
    GENERATED_BODY()

public:
    const TArray<FText>& GetOptions() const { return Options; }

    int32 GetCurrentValue() const;
    void SetValue(int32 NewValue);

private:
    TArray<FText> Options;
    TFunction<int32()> Getter;
    TFunction<void(int32)> Setter;

    void SetOptions(const TArray<FText>& NewOptions) { Options = NewOptions; }
    void AddGetter(TFunction<int32()> Func) { Getter = Func; }
    void AddSetter(TFunction<void(int32)> Func) { Setter = Func; }

    friend class USHGGameUserSettings;
};
