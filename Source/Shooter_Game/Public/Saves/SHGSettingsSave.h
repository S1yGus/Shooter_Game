// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SHGSettingsCoreTypes.h"
#include "SHGSettingsSave.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGSettingsSave : public USaveGame
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FVideoSettings VideoSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FSoundSettings SoundSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FSensitivitySettings SensitivitySettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FGameSettings GameSettings;

    friend class USHGGameUserSettings;
};
