// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/ShooterSoundOptionsUserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "ShooterGameInstance.h"
#include "ShooterSettingsSave.h"
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

UShooterSettingsSave* UShooterSoundOptionsUserWidget::GetSettingsSave()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return nullptr;

    return GameInstance->GetSettingsSave();
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
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    MasterVolumeScaleSlider->SetValue(SettingsSave->GetSoundSettings().MasterVolume);
}

void UShooterSoundOptionsUserWidget::UpdateCharacterVolumeSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    CharacterVolumeScaleSlider->SetValue(SettingsSave->GetSoundSettings().CharacterVolume);
}

void UShooterSoundOptionsUserWidget::UpdateEffectsVolumeSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    EffectsVolumeScaleSlider->SetValue(SettingsSave->GetSoundSettings().EffectsVolume);
}

void UShooterSoundOptionsUserWidget::UpdateAmbientVolumeSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    AmbientVolumeScaleSlider->SetValue(SettingsSave->GetSoundSettings().AmbientVolume);
}

void UShooterSoundOptionsUserWidget::OnSelectionChangedSoundQuality(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
        return;

    GEngine->GetGameUserSettings()->SetAudioQualityLevel(SoundQualityComboBox->FindOptionIndex(SelectedItem));
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
    const auto SettingsSave = GetSettingsSave();
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
    const auto SettingsSave = GetSettingsSave();
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
    const auto SettingsSave = GetSettingsSave();
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
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto SoundSettings = SettingsSave->GetSoundSettings();
    SoundSettings.AmbientVolume = AmbientVolumeScaleSlider->GetNormalizedValue();
    SettingsSave->SetSoundSettings(SoundSettings);
}
