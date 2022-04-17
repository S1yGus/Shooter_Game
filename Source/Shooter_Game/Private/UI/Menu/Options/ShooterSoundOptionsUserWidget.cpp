// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/ShooterSoundOptionsUserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "ShooterGameInstance.h"
#include "ShooterSettingsSaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void UShooterSoundOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    UpdateSettings();
    SetupSettingsComponents();
}

void UShooterSoundOptionsUserWidget::SetupSettingsComponents()
{
    if (SoundQualityComboBox)
    {
        SoundQualityComboBox->OnSelectionChanged.AddDynamic(this, &UShooterSoundOptionsUserWidget::OnSelectionChangedSoundQuality);
    }

    if (MasterVolumeScaleSlider)
    {
        MasterVolumeScaleSlider->OnValueChanged.AddDynamic(this, &UShooterSoundOptionsUserWidget::OnValueChangedMasterVolume);
        MasterVolumeScaleSlider->OnMouseCaptureEnd.AddDynamic(this, &UShooterSoundOptionsUserWidget::OnMouseCaptureEndMasterVolume);
    }

    if (CharacterVolumeScaleSlider)
    {
        CharacterVolumeScaleSlider->OnValueChanged.AddDynamic(this, &UShooterSoundOptionsUserWidget::OnValueChangedCharacterVolume);
        CharacterVolumeScaleSlider->OnMouseCaptureEnd.AddDynamic(this, &UShooterSoundOptionsUserWidget::OnMouseCaptureEndCharacterVolume);
    }

    if (EffectsVolumeScaleSlider)
    {
        EffectsVolumeScaleSlider->OnValueChanged.AddDynamic(this, &UShooterSoundOptionsUserWidget::OnValueChangedEffectsVolume);
        EffectsVolumeScaleSlider->OnMouseCaptureEnd.AddDynamic(this, &UShooterSoundOptionsUserWidget::OnMouseCaptureEndEffectsVolume);
    }

    if (AmbientVolumeScaleSlider)
    {
        AmbientVolumeScaleSlider->OnValueChanged.AddDynamic(this, &UShooterSoundOptionsUserWidget::OnValueChangedAmbientVolume);
        AmbientVolumeScaleSlider->OnMouseCaptureEnd.AddDynamic(this, &UShooterSoundOptionsUserWidget::OnMouseCaptureEndAmbientVolume);
    }
}

void UShooterSoundOptionsUserWidget::UpdateSettings()
{
    UpdateSoundQualityComboBox();
    UpdateMasterVolumeSlider();
    UpdateCharacterVolumeSlider();
    UpdateEffectsVolumeSlider();
    UpdateAmbientVolumeSlider();
}

void UShooterSoundOptionsUserWidget::UpdateSoundQualityComboBox()
{
    if (!SoundQualityComboBox)
        return;

    const auto SoundQuality = GEngine->GetGameUserSettings()->GetAudioQualityLevel();
    SoundQualityComboBox->SetSelectedIndex(SoundQuality);
}

void UShooterSoundOptionsUserWidget::UpdateMasterVolumeSlider()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!MasterVolumeScaleSlider || !GameInstance)
        return;

    const auto MasterVolume = GameInstance->GetSettingsSave()->GetSoundSettings().MasterVolume;
    MasterVolumeScaleSlider->SetValue(MasterVolume);
}

void UShooterSoundOptionsUserWidget::UpdateCharacterVolumeSlider()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!CharacterVolumeScaleSlider || !GameInstance)
        return;

    const auto CharacterVolume = GameInstance->GetSettingsSave()->GetSoundSettings().CharacterVolume;
    CharacterVolumeScaleSlider->SetValue(CharacterVolume);
}

void UShooterSoundOptionsUserWidget::UpdateEffectsVolumeSlider()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!EffectsVolumeScaleSlider || !GameInstance)
        return;

    const auto EffectsVolume = GameInstance->GetSettingsSave()->GetSoundSettings().EffectsVolume;
    EffectsVolumeScaleSlider->SetValue(EffectsVolume);
}

void UShooterSoundOptionsUserWidget::UpdateAmbientVolumeSlider()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!AmbientVolumeScaleSlider || !GameInstance)
        return;

    const auto AmbientVolume = GameInstance->GetSettingsSave()->GetSoundSettings().AmbientVolume;
    AmbientVolumeScaleSlider->SetValue(AmbientVolume);
}

void UShooterSoundOptionsUserWidget::OnSelectionChangedSoundQuality(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    const auto SoundQuality = SoundQualityComboBox->FindOptionIndex(SelectedItem);
    GEngine->GetGameUserSettings()->SetAudioQualityLevel(SoundQuality);
    GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UShooterSoundOptionsUserWidget::OnValueChangedMasterVolume(float Value)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return;

    UGameplayStatics::SetSoundMixClassOverride(GetWorld(),                             //
                                               GameInstance->GetDefaultSoundMix(),     //
                                               GameInstance->GetMasterSoundClass(),    //
                                               Value,                                  //
                                               1.0f,                                   //
                                               0.0f);                                  //
}

void UShooterSoundOptionsUserWidget::OnMouseCaptureEndMasterVolume()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return;

    const auto SettingsSave = GameInstance->GetSettingsSave();
    if (!SettingsSave)
        return;

    auto SoundSettings = SettingsSave->GetSoundSettings();
    SoundSettings.MasterVolume = MasterVolumeScaleSlider->GetNormalizedValue();
    SettingsSave->SetSoundSettings(SoundSettings);
}

void UShooterSoundOptionsUserWidget::OnValueChangedCharacterVolume(float Value)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return;

    UGameplayStatics::SetSoundMixClassOverride(GetWorld(),                                //
                                               GameInstance->GetDefaultSoundMix(),        //
                                               GameInstance->GetCharacterSoundClass(),    //
                                               Value,                                     //
                                               1.0f,                                      //
                                               0.0f);                                     //
}

void UShooterSoundOptionsUserWidget::OnMouseCaptureEndCharacterVolume()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return;

    const auto SettingsSave = GameInstance->GetSettingsSave();
    if (!SettingsSave)
        return;

    auto SoundSettings = SettingsSave->GetSoundSettings();
    SoundSettings.CharacterVolume = CharacterVolumeScaleSlider->GetNormalizedValue();
    SettingsSave->SetSoundSettings(SoundSettings);
}

void UShooterSoundOptionsUserWidget::OnValueChangedEffectsVolume(float Value)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return;

    UGameplayStatics::SetSoundMixClassOverride(GetWorld(),                              //
                                               GameInstance->GetDefaultSoundMix(),      //
                                               GameInstance->GetEffectsSoundClass(),    //
                                               Value,                                   //
                                               1.0f,                                    //
                                               0.0f);                                   //
}

void UShooterSoundOptionsUserWidget::OnMouseCaptureEndEffectsVolume()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return;

    const auto SettingsSave = GameInstance->GetSettingsSave();
    if (!SettingsSave)
        return;

    auto SoundSettings = SettingsSave->GetSoundSettings();
    SoundSettings.EffectsVolume = EffectsVolumeScaleSlider->GetNormalizedValue();
    SettingsSave->SetSoundSettings(SoundSettings);
}

void UShooterSoundOptionsUserWidget::OnValueChangedAmbientVolume(float Value)
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return;

    UGameplayStatics::SetSoundMixClassOverride(GetWorld(),                              //
                                               GameInstance->GetDefaultSoundMix(),      //
                                               GameInstance->GetAmbientSoundClass(),    //
                                               Value,                                   //
                                               1.0f,                                    //
                                               0.0f);                                   //
}

void UShooterSoundOptionsUserWidget::OnMouseCaptureEndAmbientVolume()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return;

    const auto SettingsSave = GameInstance->GetSettingsSave();
    if (!SettingsSave)
        return;

    auto SoundSettings = SettingsSave->GetSoundSettings();
    SoundSettings.AmbientVolume = AmbientVolumeScaleSlider->GetNormalizedValue();
    SettingsSave->SetSoundSettings(SoundSettings);
}
