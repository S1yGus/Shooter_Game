// Shooter_Game, All rights reserved.

#pragma once

#define LOCTEXT_NAMESPACE "SettingsConstants"

namespace SettingsConstants
{
// clang-format off
const FString SettingsSaveSlotName = "SettingsSave";

const FString SCMasterName         = "SC_Master";
const FString SCUIName             = "SC_UI";
const FString SCFXName             = "SC_FX";
const FString SCMusicName          = "SC_Music";

const TArray<FAspectRatioData> AspectRatioData =
    {
        {1.3333333f, 73.5f,  FText::FromString("4:3")},
        {1.7777777f, 90.0f,  FText::FromString("16:9")},
        {2.3333333f, 105.0f, FText::FromString("21:9")}
    };

const TArray<FCultureData> CultureData =
    {
        {"en",     LOCTEXT("English_Loc", "English")},
        {"ru-RU",  LOCTEXT("Russian_Loc", "Russian")}
    };
// clang-format on

const TArray<FText> FullscreenOptions    //
    {
        LOCTEXT("Fullscreen_Loc", "Fullscreen"),            //
        LOCTEXT("WindowedFullscreen_Loc", "Borderless"),    //
        LOCTEXT("Windowed_Loc", "Windowed")                 //
    };

const TArray<FText> VSyncOptions    //
    {
        LOCTEXT("VSyncDisabled_Loc", "Disabled"),    //
        LOCTEXT("VSyncEnabled_Loc", "Enabled")       //
    };

const TArray<FText> FramerateOptions    //
    {
        LOCTEXT("FramerateUnlimited_Loc", "Unlimited"),    //
        FText::FromString("30"),                           //
        FText::FromString("60"),                           //
        FText::FromString("120"),                          //
        FText::FromString("144")                           //
    };

const TArray<FText> GraphicsQualityOptions    //
    {
        LOCTEXT("GraphicsQualityLow_Loc", "Low"),               //
        LOCTEXT("GraphicsQualityMedium_Loc", "Medium"),         //
        LOCTEXT("GraphicsQualityHigh_Loc", "High"),             //
        LOCTEXT("GraphicsQualityEpic_Loc", "Epic"),             //
        LOCTEXT("GraphicsQualityCinematic_Loc", "Cinematic")    //
    };

const TArray<FText> SoundQualityOptions    //
    {
        LOCTEXT("SoundQualityLow_Loc", "Low"),          //
        LOCTEXT("SoundQualityMedium_Loc", "Medium"),    //
        LOCTEXT("SoundQualityHigh_Loc", "High"),        //
    };
}    // namespace SettingsConstants

#undef LOCTEXT_NAMESPACE
