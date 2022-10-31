// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SHGSetting.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGSetting : public UObject
{
    GENERATED_BODY()

public:
    const FText& GetName() const { return Name; }

private:
    FText Name;

    void SetName(const FText& NewName) { Name = NewName; }

    friend class USHGGameUserSettings;
};
