// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/SHGSetting.h"
#include "SHGActionSetting.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGActionSetting : public USHGSetting
{
    GENERATED_BODY()

public:
    void DoAction();
    bool IsActionAvailable();
    const FText& GetActionName() const { return ActionName; }

private:
    FText ActionName;
    TFunction<void()> Action;
    TFunction<bool()> Status;

    void SetActionName(const FText& NewActionName) { ActionName = NewActionName; }
    void AddActionFunc(TFunction<void()> Func) { Action = Func; }
    void AddStatusFunc(TFunction<bool()> Func) { Status = Func; }

    friend class USHGGameUserSettings;
};
