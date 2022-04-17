// Shooter_Game, All rights reserved.

#include "ShooterGameInstance.h"
#include "ShooterSettingsSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UShooterGameInstance::SaveSettingsSave()
{
    if (!SettingsSave)
        return;

    UGameplayStatics::SaveGameToSlot(SettingsSave, SettingsSaveSlotName, 0);
}

void UShooterGameInstance::OnStart()
{
    Super::OnStart();

    CheckSoundSettings();
}

void UShooterGameInstance::CheckSoundSettings()
{
    if (UGameplayStatics::DoesSaveGameExist(SettingsSaveSlotName, 0))
    {
        SettingsSave = Cast<UShooterSettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlotName, 0));
    }
    else
    {
        SettingsSave = Cast<UShooterSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UShooterSettingsSaveGame::StaticClass()));
    }

    if (SettingsSave)
    {
        ApplySoundSettings();
    }
}

void UShooterGameInstance::ApplySoundSettings()
{
    if (!DefaultSoundMix)
        return;

    const auto SoundSettings = SettingsSave->GetSoundSettings();
    if (MasterSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, MasterSoundClass, SoundSettings.MasterVolume, 1.0f, 0.0f);
    }

    if (CharacterSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, CharacterSoundClass, SoundSettings.CharacterVolume, 1.0f, 0.0f);
    }

    if (EffectsSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, EffectsSoundClass, SoundSettings.EffectsVolume, 1.0f, 0.0f);
    }

    if (AmbientSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(GetWorld(), DefaultSoundMix, AmbientSoundClass, SoundSettings.AmbientVolume, 1.0f, 0.0f);
    }
}