// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameInstance.generated.h"

class UShooterSettingsSave;
class USoundMix;
class USoundClass;

UCLASS()
class SHOOTER_GAME_API UShooterGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UShooterSettingsSave* GetSettingsSave() const { return SettingsSave; }
    void SaveSettingsSave();

    USoundMix* GetDefaultSoundMix() const { return DefaultSoundMix; }
    USoundClass* GetMasterSoundClass() const { return MasterSoundClass; }
    USoundClass* GetCharacterSoundClass() const { return CharacterSoundClass; }
    USoundClass* GetEffectsSoundClass() const { return EffectsSoundClass; }
    USoundClass* GetAmbientSoundClass() const { return AmbientSoundClass; }

    FLevelData GetNewGameLevelData() const { return NewGameLevelData; }
    void SetNewGameLevelData(const FLevelData& Data) { NewGameLevelData = Data; }
    const TArray<FLevelData>& GetGameLevelsData() const { return GameLevelsData; }
    FName GetMenuLevelName() const { return MenuLevelName; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TArray<FLevelData> GameLevelsData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName MenuLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FString SettingsSaveSlotName = "SettingsSave";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundMix* DefaultSoundMix = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundClass* MasterSoundClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundClass* CharacterSoundClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundClass* EffectsSoundClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Sound")
    USoundClass* AmbientSoundClass = nullptr;

    virtual void OnStart() override;

private:
    UPROPERTY()
    UShooterSettingsSave* SettingsSave = nullptr;

    FLevelData NewGameLevelData;

    void CheckSoundSettings();
    void ApplySoundSettings();
};
