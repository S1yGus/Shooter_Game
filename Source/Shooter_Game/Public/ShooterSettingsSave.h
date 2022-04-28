// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShooterCoreTypes.h"
#include "ShooterSettingsSave.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterSettingsSave : public USaveGame
{
    GENERATED_BODY()

public:
    FSoundSettings GetSoundSettings() const { return SoundSettings; }
    void SetSoundSettings(const FSoundSettings& NewSoundSettings) { SoundSettings = NewSoundSettings; }

    FControlSettings GetControlSettings() const { return ControlSettings; }
    void SetControlSettings(const FControlSettings& NewControlSettings) { ControlSettings = NewControlSettings; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FSoundSettings SoundSettings;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FControlSettings ControlSettings;
};
