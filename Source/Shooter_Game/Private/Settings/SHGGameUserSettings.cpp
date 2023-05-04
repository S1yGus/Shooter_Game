// Shooter_Game, All rights reserved.

#include "Settings/SHGGameUserSettings.h"
#include "Settings/SHGSetting.h"
#include "Settings/SHGIntSetting.h"
#include "Settings/SHGFloatSetting.h"
#include "Settings/SHGActionSetting.h"
#include "Settings/SHGBoolSetting.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Saves/SHGSettingsSave.h"
#include "AudioDevice.h"
#include "AudioThread.h"
#include "Settings/SHGSettingsConstants.h"

using namespace SettingsConstants;

constexpr static float MinMouseSens = 0.01f;

#define LOCTEXT_NAMESPACE "GameUserSettings"

#define BIND_VIDEO_SETTING(SettingName)   \
    Setting->AddGetter(                   \
        [&]()                             \
        {                                 \
            return Get##SettingName();    \
        });                               \
    Setting->AddSetter(                   \
        [&](int32 NewValue)               \
        {                                 \
            Set##SettingName(NewValue);   \
            ApplyNonResolutionSettings(); \
        });

#define BIND_SOUND_SETTING(SCName, VolumeType)                 \
    Setting->AddGetter(                                        \
        [&]()                                                  \
        {                                                      \
            return SettingsSave->SoundSettings.VolumeType;     \
        });                                                    \
    Setting->AddSetter(                                        \
        [&](float NewValue)                                    \
        {                                                      \
            SetSoundClassVolume(SCName, NewValue);             \
            SettingsSave->SoundSettings.VolumeType = NewValue; \
        });

#define BIND_CONTROL_SETTINGS(SensitivityType)                                                                         \
    Setting->AddGetter(                                                                                                \
        [&]()                                                                                                          \
        {                                                                                                              \
            return SettingsSave->SensitivitySettings.SensitivityType;                                                  \
        });                                                                                                            \
    Setting->AddSetter(                                                                                                \
        [&](float NewValue)                                                                                            \
        {                                                                                                              \
            const auto InvertMultiplier = SettingsSave->SensitivitySettings.SensitivityType < 0 ? -1.0f : 1.0f;        \
            SettingsSave->SensitivitySettings.SensitivityType = FMath::Max(NewValue, MinMouseSens) * InvertMultiplier; \
            OnSensitivityChanged.Broadcast(SettingsSave->SensitivitySettings);                                         \
        });

static bool operator==(const FText& Text1, const FText& Text2)
{
    return Text1.ToString() == Text2.ToString();
}

static bool operator==(const FCultureData& Data, const FString& Str)
{
    return Data.Culture == Str;
}

USHGGameUserSettings::USHGGameUserSettings()
{
    InitVideoSettings();
    InitSoundSettings();
    InitControlSettings();
    InitGameSettings();
}

USHGGameUserSettings* USHGGameUserSettings::Get()
{
    return GEngine ? Cast<USHGGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

const FAspectRatioData& USHGGameUserSettings::GetAspectRatio() const
{
    return SettingsSave->VideoSettings.AspectRatioData;
}

const FSensitivitySettings& USHGGameUserSettings::GetSensitivitySettings() const
{
    return SettingsSave->SensitivitySettings;
}

const TMap<EHintType, bool>& USHGGameUserSettings::GetHintsStatus() const
{
    return SettingsSave->GameSettings.HintsStatus;
}

float USHGGameUserSettings::GetDifficulty() const
{
    return SettingsSave->GameSettings.Difficulty;
}

void USHGGameUserSettings::SetHintsStatus(const TMap<EHintType, bool>& NewHintsMap)
{
    SettingsSave->GameSettings.HintsStatus = NewHintsMap;
    SaveSettings();
}

void USHGGameUserSettings::SetLastConfirmedResolutionSettings()
{
    SetFullscreenMode(GetLastConfirmedFullscreenMode());
    SetScreenResolution(GetLastConfirmedScreenResolution());
    ApplyResolutionSettings(false);
    UpdateResolutionSetting();
}

void USHGGameUserSettings::InitSoundVolume()
{
    SetSoundClassVolume(SCMasterName, SettingsSave->SoundSettings.MasterVolume);
    SetSoundClassVolume(SCUIName, SettingsSave->SoundSettings.UIVolume);
    SetSoundClassVolume(SCFXName, SettingsSave->SoundSettings.FXVolume);
    SetSoundClassVolume(SCMusicName, SettingsSave->SoundSettings.MusicVolume);
}

void USHGGameUserSettings::LoadSettings(bool bForceReload)
{
    Super::LoadSettings(bForceReload);

    CheckSettingsSave();
}

void USHGGameUserSettings::SaveSettings()
{
    Super::SaveSettings();

    UGameplayStatics::SaveGameToSlot(SettingsSave, SettingsSaveSlotName, 0);
}

void USHGGameUserSettings::InitVideoSettings()
{
    auto& VideoSettings = SettingsData[SettingsData.Emplace(ESettingType::Video)].Settings;

    {
        auto Setting = CreateIntSetting(LOCTEXT("ScreenMode_Loc", "Screen mode"), FullscreenOptions, VideoSettings);
        Setting->AddGetter(
            [&]()
            {
                return static_cast<int32>(GetFullscreenMode());
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SetFullscreenMode(static_cast<EWindowMode::Type>(NewValue));
                ApplyResolutionSettings(false);
                SetLowestResolution();
            });
    }

    {
        ResolutionSetting = CreateIntSetting(LOCTEXT("Resolution_Loc", "Resolution"), GetScreenResolutions(), VideoSettings);
        ResolutionSetting->AddGetter(
            [&]() -> int32
            {
                if (GetFullscreenMode() == EWindowMode::WindowedFullscreen)
                {
                    return INDEX_NONE;
                }
                const auto CurrentResolution = GetScreenResolution();
                const auto Option = FText::FromString(FString::Printf(TEXT("%d x %d"), CurrentResolution.X, CurrentResolution.Y));
                return ResolutionSetting->GetOptions().IndexOfByKey(Option);
            });
        ResolutionSetting->AddSetter(
            [&](int32 NewValue)
            {
                FString LeftS, RightS;
                if (ResolutionSetting->GetOptions()[NewValue].ToString().Split(" x ", &LeftS, &RightS))
                {
                    FIntPoint NewResolution = FIntPoint(FCString::Atoi(*LeftS), FCString::Atoi(*RightS));
                    SetScreenResolution(NewResolution);
                    ApplyResolutionSettings(false);
                    OnResolutionChanged.Broadcast();
                }
            });
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("VSync_Loc", "V-Sync"), VSyncOptions, VideoSettings);
        Setting->AddGetter(
            [&]()
            {
                return static_cast<int32>(IsVSyncEnabled());
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SetVSyncEnabled(static_cast<bool>(NewValue));
                ApplyNonResolutionSettings();
            });
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("FramerateLimit_Loc", "Framerate limit"), FramerateOptions, VideoSettings);
        Setting->AddGetter(
            [&]()
            {
                return GetFrameRateLimit() == 0.0f ? 0    //
                                                   : FramerateOptions.IndexOfByKey(FText::AsNumber(static_cast<int32>(GetFrameRateLimit())));
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SetFrameRateLimit(NewValue == 0 ? 0.0f    //
                                                : FCString::Atof(*FramerateOptions[NewValue].ToString()));
                ApplyNonResolutionSettings();
            });
    }

    {
        TArray<FText> AspectRatioOptions;
        for (const auto& Data : AspectRatioData)
        {
            AspectRatioOptions.Add(Data.DisplayName);
        }

        auto Setting = CreateIntSetting(LOCTEXT("AspectRatio_Loc", "Aspect ratio"), AspectRatioOptions, VideoSettings);
        Setting->AddGetter(
            [&, AspectRatioOptions]() -> int32
            {
                if (GetFullscreenMode() == EWindowMode::Windowed || GetFullscreenMode() == EWindowMode::WindowedFullscreen)
                {
                    return INDEX_NONE;
                }
                return AspectRatioOptions.IndexOfByKey(SettingsSave->VideoSettings.AspectRatioData.DisplayName);
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SettingsSave->VideoSettings.AspectRatioData = AspectRatioData[NewValue];
                OnAspectRatioChanged.Broadcast(AspectRatioData[NewValue]);
            });
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("ViewDistanceQuality_Loc", "View distance"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(ViewDistanceQuality);
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("AntiAliasingQuality_Loc", "Anti aliasing"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(AntiAliasingQuality);
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("PostProcessingQuality_Loc", "Post processing"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(PostProcessingQuality);
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("ShadowsQuality_Loc", "Shadows"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(ShadowQuality);
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("GlobalIlluminationQuality_Loc", "Global illumination"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(GlobalIlluminationQuality);
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("ReflectionsQuality_Loc", "Reflections"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(ReflectionQuality);
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("TexturesQuality_Loc", "Textures"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(TextureQuality);
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("VisualEffectsQuality_Loc", "Visual effects"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(VisualEffectQuality);
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("FoliageQuality_Loc", "Foliage"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(FoliageQuality);
    }

    {
        auto Setting = CreateIntSetting(LOCTEXT("ShadingQuality_Loc", "Shading"), GraphicsQualityOptions, VideoSettings);
        BIND_VIDEO_SETTING(ShadingQuality);
    }
}

void USHGGameUserSettings::InitSoundSettings()
{
    auto& SoundSettings = SettingsData[SettingsData.Emplace(ESettingType::Sound)].Settings;

    {
        auto Setting = CreateIntSetting(LOCTEXT("SoundQuality_Loc", "Sound quality"), SoundQualityOptions, SoundSettings);
        Setting->AddGetter(
            [&]()
            {
                return GetAudioQualityLevel();
            });
        Setting->AddSetter(
            [&](int32 NewValue)
            {
                SetAudioQualityLevel(NewValue);
            });
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("MasterVolume_Loc", "Master"), SoundSettings);
        BIND_SOUND_SETTING(SCMasterName, MasterVolume);
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("UIVolume_Loc", "Interface"), SoundSettings);
        BIND_SOUND_SETTING(SCUIName, UIVolume);
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("FXVolume_Loc", "Effects"), SoundSettings);
        BIND_SOUND_SETTING(SCFXName, FXVolume);
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("MusicVolume_Loc", "Music"), SoundSettings);
        BIND_SOUND_SETTING(SCMusicName, MusicVolume);
    }
}

void USHGGameUserSettings::InitControlSettings()
{
    auto& ControlSettings = SettingsData[SettingsData.Emplace(ESettingType::Control)].Settings;

    {
        auto Setting = CreateBoolSetting(LOCTEXT("InvertMouseY_Loc", "Invert Y"), ControlSettings);
        Setting->AddGetter(
            [&]()
            {
                return SettingsSave->SensitivitySettings.MouseYSens < 0.0f && SettingsSave->SensitivitySettings.MouseAimedYSens < 0.0f;
            });
        Setting->AddSetter(
            [&](bool bNewValue)
            {
                if ((bNewValue && SettingsSave->SensitivitySettings.MouseYSens > 0.0f) || (!bNewValue && SettingsSave->SensitivitySettings.MouseYSens < 0.0f))
                {
                    SettingsSave->SensitivitySettings.MouseYSens *= -1.0f;
                }
                if ((bNewValue && SettingsSave->SensitivitySettings.MouseAimedYSens > 0.0f) || (!bNewValue && SettingsSave->SensitivitySettings.MouseAimedYSens < 0.0f))
                {
                    SettingsSave->SensitivitySettings.MouseAimedYSens *= -1.0f;
                }
                OnSensitivityChanged.Broadcast(SettingsSave->SensitivitySettings);
            });
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("MouseXSens_Loc", "X sensitivity"), ControlSettings);
        BIND_CONTROL_SETTINGS(MouseXSens);
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("MouseYSens_Loc", "Y sensitivity"), ControlSettings);
        BIND_CONTROL_SETTINGS(MouseYSens);
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("MouseAimedXSens_Loc", "Aimed X sens."), ControlSettings);
        BIND_CONTROL_SETTINGS(MouseAimedXSens);
    }

    {
        auto Setting = CreateFloatSetting(LOCTEXT("MouseAimedYSens_Loc", "Aimed Y sens."), ControlSettings);
        BIND_CONTROL_SETTINGS(MouseAimedYSens);
    }
}

void USHGGameUserSettings::InitGameSettings()
{
    auto& GameSettings = SettingsData[SettingsData.Emplace(ESettingType::Game)].Settings;

    {
        TArray<FText> LanguageOptions;
        for (const auto& Culture : CultureData)
        {
            LanguageOptions.Add(Culture.CultureName);
        }

        auto Setting = CreateIntSetting(LOCTEXT("Language_Loc", "Language"), LanguageOptions, GameSettings);
        Setting->AddGetter(
            []()
            {
                return CultureData.IndexOfByKey(UKismetInternationalizationLibrary::GetCurrentCulture());
            });
        Setting->AddSetter(
            [](int32 NewValue)
            {
                UKismetInternationalizationLibrary::SetCurrentCulture(CultureData[NewValue].Culture, true);
            });
    }

    {
        auto Setting = CreateActionSetting(LOCTEXT("ResetHints_Loc", "Reset hints"), LOCTEXT("ResetHintsButton_Loc", "RESET"), GameSettings);
        Setting->AddActionFunc(
            [&]()
            {
                for (auto& [HintType, HintStatus] : SettingsSave->GameSettings.HintsStatus)
                {
                    HintStatus = true;
                }

                OnHintsStatusChanged.Broadcast(SettingsSave->GameSettings.HintsStatus);
            });
        Setting->AddStatusFunc(
            [&]()
            {
                for (const auto& [HintType, HintStatus] : SettingsSave->GameSettings.HintsStatus)
                {
                    if (!HintStatus)    // If hint already have been shown.
                    {
                        return true;
                    }
                }

                return false;
            });
    }
}

TArray<FText> USHGGameUserSettings::GetScreenResolutions() const
{
    const auto ScreenMode = GetFullscreenMode();

    TArray<FIntPoint> Resolutions;
    if (ScreenMode == EWindowMode::Windowed)
    {
        UKismetSystemLibrary::GetConvenientWindowedResolutions(Resolutions);
    }
    else
    {
        UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
    }

    TArray<FText> ResolutionsOptions;
    for (const auto& Resolution : Resolutions)
    {
        ResolutionsOptions.Add(FText::FromString(FString::Printf(TEXT("%d x %d"), Resolution.X, Resolution.Y)));
    }

    return ResolutionsOptions;
}

void USHGGameUserSettings::UpdateResolutionSetting()
{
    ResolutionSetting->SetOptions(GetScreenResolutions());
}

void USHGGameUserSettings::SetLowestResolution()
{
    UpdateResolutionSetting();

    if (GetFullscreenMode() != EWindowMode::Type::WindowedFullscreen)
    {
        ResolutionSetting->SetValue(0);    // Set the lowest resolution in this screen mode.
    }
    else
    {
        OnResolutionChanged.Broadcast();
    }
}

void USHGGameUserSettings::SetSoundClassVolume(const FString& SoundClassName, float NewVolume)
{
    FAudioThread::RunCommandOnAudioThread(
        [=]()
        {
            if (!GEngine)
                return;

            auto ADevice = GEngine->GetMainAudioDevice();
            if (!ADevice)
                return;

            for (const auto& [SoundClass, SoundClassProperties] : ADevice->GetSoundClassPropertyMap())
            {
                if (SoundClass && SoundClass->GetFullName().Find(SoundClassName) != INDEX_NONE)
                {
                    SoundClass->Properties.Volume = NewVolume;
                }
            }
        });
}

void USHGGameUserSettings::CheckSettingsSave()
{
    if (SettingsSave)
        return;

    if (UGameplayStatics::DoesSaveGameExist(SettingsSaveSlotName, 0))
    {
        SettingsSave = Cast<USHGSettingsSave>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlotName, 0));
    }
    else
    {
        SettingsSave = Cast<USHGSettingsSave>(UGameplayStatics::CreateSaveGameObject(USHGSettingsSave::StaticClass()));
        RunHardwareBenchmark();
        ApplySettings(false);
    }

    check(SettingsSave);
}

USHGIntSetting* USHGGameUserSettings::CreateIntSetting(const FText& Name, const TArray<FText>& Options, TArray<USHGSetting*>& AddTo)
{
    const auto Setting = CreateSetting<USHGIntSetting>(Name, AddTo);
    Setting->SetOptions(Options);

    return Setting;
}

USHGFloatSetting* USHGGameUserSettings::CreateFloatSetting(const FText& Name, TArray<USHGSetting*>& AddTo)
{
    return CreateSetting<USHGFloatSetting>(Name, AddTo);
}

USHGActionSetting* USHGGameUserSettings::CreateActionSetting(const FText& Name, const FText& ActionName, TArray<USHGSetting*>& AddTo)
{
    const auto Setting = CreateSetting<USHGActionSetting>(Name, AddTo);
    Setting->SetActionName(ActionName);

    return Setting;
}

USHGBoolSetting* USHGGameUserSettings::CreateBoolSetting(const FText& Name, TArray<USHGSetting*>& AddTo)
{
    return CreateSetting<USHGBoolSetting>(Name, AddTo);
}

#undef LOCTEXT_NAMESPACE
