// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShooterCoreTypes.h"
#include "ShooterSettingsSaveGame.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterSettingsSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    FSoundSettings GetSoundSettings() const { return SoundSettings; }
    void SetSoundSettings(const FSoundSettings& NewSoundSettings) { SoundSettings = NewSoundSettings; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    FSoundSettings SoundSettings;
};
