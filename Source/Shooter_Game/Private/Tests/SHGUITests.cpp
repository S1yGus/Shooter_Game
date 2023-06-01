// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGUITests.h"
#include "CoreMinimal.h"
#include "Tests/Utils/SHGTestsUtils.h"
#include "Misc/AutomationTest.h"
#include "UI/Menu/SHGPauseUserWidget.h"
#include "UI/Menu/SHGButtonUserWidget.h"
#include "UI/Menu/Options/SHGOptionsMenuUserWidget.h"
#include "UI/Menu/Main/SHGMenuUserWidget.h"
#include "UI/Menu/Options/SHGOptionsButtonUserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Settings/SHGGameUserSettings.h"
#include "Settings/SHGIntSetting.h"
#include "Settings/SHGFloatSetting.h"
#include "Settings/SHGBoolSetting.h"
#include "Kismet/GameplayStatics.h"

using namespace Tests;

namespace
{
const char* SandboxLevelName{"/Game/Levels/SandboxLevel"};
const char* MenuLevelName{"/Game/Levels/MenuLevel"};
}    // namespace

static void SetComboBoxNextOption(ESettingType SettingType, uint8 WidgetNumber)
{
    auto* OptionsMenuUserWidget = FindWidgetByClass<USHGOptionsMenuUserWidget>();
    if (!OptionsMenuUserWidget)
        return;

    const auto* OptionsWidgetSwitcher = FindWidgetByName<UWidgetSwitcher>(OptionsMenuUserWidget, "OptionsWidgetSwitcher");
    if (!OptionsWidgetSwitcher)
        return;

    auto* SettingsOptionsWidget = Cast<UUserWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(static_cast<int32>(SettingType)));
    const auto* SettingsVerticalBox = FindWidgetByName<UVerticalBox>(SettingsOptionsWidget, "SettingsVerticalBox");
    if (!SettingsVerticalBox)
        return;

    auto* SettingWidget = Cast<UUserWidget>(SettingsVerticalBox->GetChildAt(WidgetNumber));
    if (!SettingWidget)
        return;

    auto* SettingComboBox = FindWidgetByName<UComboBoxString>(SettingWidget, "SettingComboBox");
    if (!SettingComboBox)
        return;

    const auto Index = (SettingComboBox->GetSelectedIndex() + 1) % SettingComboBox->GetOptionCount();
    SettingComboBox->OnSelectionChanged.Broadcast(SettingComboBox->GetOptionAtIndex(Index), ESelectInfo::OnMouseClick);
}
static void SetSliderNextValue(ESettingType SettingType, uint8 WidgetNumber, float NextValue)
{
    auto* OptionsMenuUserWidget = FindWidgetByClass<USHGOptionsMenuUserWidget>();
    if (!OptionsMenuUserWidget)
        return;

    const auto* OptionsWidgetSwitcher = FindWidgetByName<UWidgetSwitcher>(OptionsMenuUserWidget, "OptionsWidgetSwitcher");
    if (!OptionsWidgetSwitcher)
        return;

    auto* SettingsOptionsWidget = Cast<UUserWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(static_cast<int32>(SettingType)));
    const auto* SettingsVerticalBox = FindWidgetByName<UVerticalBox>(SettingsOptionsWidget, "SettingsVerticalBox");
    if (!SettingsVerticalBox)
        return;

    auto* SettingWidget = Cast<UUserWidget>(SettingsVerticalBox->GetChildAt(WidgetNumber));
    if (!SettingWidget)
        return;

    auto* SettingSlider = FindWidgetByName<USlider>(SettingWidget, "SettingSlider");
    if (!SettingSlider)
        return;

    SettingSlider->SetValue(NextValue);
    SettingSlider->OnMouseCaptureEnd.Broadcast();
}
static void ToggleCheckBox(ESettingType SettingType, uint8 WidgetNumber)
{
    auto* OptionsMenuUserWidget = FindWidgetByClass<USHGOptionsMenuUserWidget>();
    if (!OptionsMenuUserWidget)
        return;

    const auto* OptionsWidgetSwitcher = FindWidgetByName<UWidgetSwitcher>(OptionsMenuUserWidget, "OptionsWidgetSwitcher");
    if (!OptionsWidgetSwitcher)
        return;

    auto* SettingsOptionsWidget = Cast<UUserWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(static_cast<int32>(SettingType)));
    const auto* SettingsVerticalBox = FindWidgetByName<UVerticalBox>(SettingsOptionsWidget, "SettingsVerticalBox");
    if (!SettingsVerticalBox)
        return;

    auto* SettingWidget = Cast<UUserWidget>(SettingsVerticalBox->GetChildAt(WidgetNumber));
    if (!SettingWidget)
        return;

    auto* SettingCheckBox = FindWidgetByName<UCheckBox>(SettingWidget, "SettingCheckBox");
    if (!SettingCheckBox)
        return;

    SettingCheckBox->SetIsChecked(!SettingCheckBox->IsChecked());
    SettingCheckBox->OnCheckStateChanged.Broadcast(SettingCheckBox->IsChecked());
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeVisibleOnGamePaused, "Shooter_Game.UI.InGameMenu.PauseMenuShouldBeVisibleOnGamePaused",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeCollapsedOnGameUnpaused, "Shooter_Game.UI.InGameMenu.PauseMenuShouldBeCollapsedOnGameUnpaused",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeCollapsedWhenClickedResumeButton, "Shooter_Game.UI.InGameMenu.PauseMenuShouldBeCollapsedWhenClickedResumeButton",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOptionsMenuShouldBeVisibleWhenClickedOptionsButton, "Shooter_Game.UI.InGameMenu.OptionsMenuShouldBeVisibleWhenClickedOptionsButton",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMenuLevelShouldBeLoadedWhenClickedMenuButton, "Shooter_Game.UI.InGameMenu.MenuLevelShouldBeLoadedWhenClickedMenuButton",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameLevelShouldBeLoadedWhenClickedNewGameButton, "Shooter_Game.UI.MainMenu.GameLevelShouldBeLoadedWhenClickedNewGameButton",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOptionsMenuShouldBeVisibleWhenClickedOptionsButtonInMainMenu,
                                 "Shooter_Game.UI.MainMenu.OptionsMenuShouldBeVisibleWhenClickedOptionsButtonInMainMenu",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOptionsWidgetShouldBeChangedWhenClickedOptionsButtons, "Shooter_Game.UI.OptionsWidgetShouldBeChangedWhenClickedOptionsButtons",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllSettingsShouldExist, "Shooter_Game.UI.AllSettingsShouldExist",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComboBoxSettingCanBeApplied, "Shooter_Game.UI.ComboBoxSettingCanBeApplied",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSliderSettingCanBeApplied, "Shooter_Game.UI.SliderSettingCanBeApplied",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCheckBoxSettingCanBeApplied, "Shooter_Game.UI.CheckBoxSettingCanBeApplied",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FPauseMenuShouldBeVisibleOnGamePaused::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    const UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    const auto* PauseUserWidget = FindWidgetByClass<USHGPauseUserWidget>();
    if (!TestNotNull("PauseUserWidget must exists.", PauseUserWidget))
        return false;

    TestTrueExpr(PauseUserWidget->GetVisibility() == ESlateVisibility::Collapsed);

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
        },
        0.66f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TestTrueExpr(PauseUserWidget->GetVisibility() == ESlateVisibility::Visible);
        },
        0.51f));

    return true;
}

bool FPauseMenuShouldBeCollapsedOnGameUnpaused::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    const UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    const auto* PauseUserWidget = FindWidgetByClass<USHGPauseUserWidget>();
    if (!TestNotNull("PauseUserWidget must exists.", PauseUserWidget))
        return false;

    TestTrueExpr(PauseUserWidget->GetVisibility() == ESlateVisibility::Collapsed);

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
        },
        0.66f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
        },
        0.82f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TestTrueExpr(PauseUserWidget->GetVisibility() == ESlateVisibility::Collapsed);
        },
        0.31f));

    return true;
}

bool FPauseMenuShouldBeCollapsedWhenClickedResumeButton::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    const UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    auto* PauseUserWidget = FindWidgetByClass<USHGPauseUserWidget>();
    if (!TestNotNull("PauseUserWidget must exists.", PauseUserWidget))
        return false;

    const auto* ResumeButton = FindWidgetByName<USHGButtonUserWidget>(PauseUserWidget, "ResumeButton");
    if (!TestNotNull("ResumeButton must exists.", ResumeButton))
        return false;

    TestTrueExpr(PauseUserWidget->GetVisibility() == ESlateVisibility::Collapsed && !PC->IsPaused());

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
        },
        0.66f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            ResumeButton->OnClickedButton.Broadcast();
        },
        0.82f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TestTrueExpr(PauseUserWidget->GetVisibility() == ESlateVisibility::Collapsed && !PC->IsPaused());
        },
        0.31f));

    return true;
}

bool FOptionsMenuShouldBeVisibleWhenClickedOptionsButton::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    const UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    auto* PauseUserWidget = FindWidgetByClass<USHGPauseUserWidget>();
    if (!TestNotNull("PauseUserWidget must exists.", PauseUserWidget))
        return false;

    const auto* OptionsButton = FindWidgetByName<USHGButtonUserWidget>(PauseUserWidget, "OptionsButton");
    if (!TestNotNull("OptionsButton must exists.", OptionsButton))
        return false;

    const auto* OptionsMenuUserWidget = FindWidgetByClass<USHGOptionsMenuUserWidget>();
    if (!TestNotNull("OptionsMenuUserWidget must exists.", OptionsMenuUserWidget))
        return false;

    TestTrueExpr(OptionsMenuUserWidget->GetVisibility() == ESlateVisibility::Collapsed);

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
        },
        0.66f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            OptionsButton->OnClickedButton.Broadcast();
        },
        0.82f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TestTrueExpr(OptionsMenuUserWidget->GetVisibility() == ESlateVisibility::Visible);
        },
        0.31f));

    return true;
}

bool FMenuLevelShouldBeLoadedWhenClickedMenuButton::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    const UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    auto* PauseUserWidget = FindWidgetByClass<USHGPauseUserWidget>();
    if (!TestNotNull("PauseUserWidget must exists.", PauseUserWidget))
        return false;

    const auto* MenuButton = FindWidgetByName<USHGButtonUserWidget>(PauseUserWidget, "MenuButton");
    if (!TestNotNull("MenuButton must exists.", MenuButton))
        return false;

    TestTrueExpr(UGameplayStatics::GetCurrentLevelName(World).Equals("SandboxLevel"));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
        },
        0.66f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            MenuButton->OnClickedButton.Broadcast();
        },
        0.82f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [&]()
        {
            TestTrueExpr(UGameplayStatics::GetCurrentLevelName(GetCurrentWorld()).Equals("MenuLevel"));
        },
        2.0f));

    return true;
}

bool FGameLevelShouldBeLoadedWhenClickedNewGameButton::RunTest(const FString& Parameters)
{
    const LevelScope Level(MenuLevelName);

    auto* MainMenuUserWidget = FindWidgetByClass<USHGMenuUserWidget>();
    if (!TestNotNull("MainMenuUserWidget must exists.", MainMenuUserWidget))
        return false;

    const auto* NewGameButton = FindWidgetByName<USHGButtonUserWidget>(MainMenuUserWidget, "NewGameButton");
    if (!TestNotNull("NewGameButton must exists.", NewGameButton))
        return false;

    TestTrueExpr(UGameplayStatics::GetCurrentLevelName(GetCurrentWorld()).Equals("MenuLevel"));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            NewGameButton->OnClickedButton.Broadcast();
        },
        0.31f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [&]()
        {
            TestTrueExpr(!UGameplayStatics::GetCurrentLevelName(GetCurrentWorld()).Equals("MenuLevel"));
        },
        2.0f));

    return true;
}

bool FOptionsMenuShouldBeVisibleWhenClickedOptionsButtonInMainMenu::RunTest(const FString& Parameters)
{
    const LevelScope Level(MenuLevelName);

    auto* MainMenuUserWidget = FindWidgetByClass<USHGMenuUserWidget>();
    if (!TestNotNull("MainMenuUserWidget must exists.", MainMenuUserWidget))
        return false;

    const auto* OptionsButton = FindWidgetByName<USHGButtonUserWidget>(MainMenuUserWidget, "OptionsButton");
    if (!TestNotNull("OptionsButton must exists.", OptionsButton))
        return false;

    const auto* OptionsMenuUserWidget = FindWidgetByClass<USHGOptionsMenuUserWidget>();
    if (!TestNotNull("OptionsMenuUserWidget must exists.", OptionsMenuUserWidget))
        return false;

    TestTrueExpr(OptionsMenuUserWidget->GetVisibility() == ESlateVisibility::Collapsed);

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            OptionsButton->OnClickedButton.Broadcast();
        },
        0.31f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TestTrueExpr(OptionsMenuUserWidget->GetVisibility() == ESlateVisibility::Visible);
        },
        0.31f));

    return true;
}

bool FOptionsWidgetShouldBeChangedWhenClickedOptionsButtons::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    const UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    auto* PauseUserWidget = FindWidgetByClass<USHGPauseUserWidget>();
    if (!TestNotNull("PauseUserWidget must exists.", PauseUserWidget))
        return false;

    const auto* OptionsButton = FindWidgetByName<USHGButtonUserWidget>(PauseUserWidget, "OptionsButton");
    if (!TestNotNull("OptionsButton must exists.", OptionsButton))
        return false;

    auto* OptionsMenuUserWidget = FindWidgetByClass<USHGOptionsMenuUserWidget>();
    if (!TestNotNull("OptionsMenuUserWidget must exists.", OptionsMenuUserWidget))
        return false;

    const auto* OptionsWidgetSwitcher = FindWidgetByName<UWidgetSwitcher>(OptionsMenuUserWidget, "OptionsWidgetSwitcher");
    if (!TestNotNull("OptionsWidgetSwitcher must exists.", OptionsWidgetSwitcher))
        return false;

    const auto* OptionsButtonsBox = FindWidgetByName<UVerticalBox>(OptionsMenuUserWidget, "OptionsButtonsBox");
    if (!TestNotNull("OptionsButtonsBox must exists.", OptionsButtonsBox))
        return false;

    const int32 SoundOptionsButtonNumber = 1;
    auto* SoundOptionsButton = Cast<USHGOptionsButtonUserWidget>(OptionsButtonsBox->GetChildAt(SoundOptionsButtonNumber));
    if (!TestNotNull("SoundOptionsButton must exists.", SoundOptionsButton))
        return false;

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
        },
        0.66f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            OptionsButton->OnClickedButton.Broadcast();
        },
        0.82f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            SoundOptionsButton->OnClickedOptionsButton.Broadcast(SoundOptionsButton->GetWidgetID());
        },
        0.62f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TestTrueExpr(OptionsWidgetSwitcher->GetActiveWidgetIndex() == SoundOptionsButtonNumber);
        },
        0.26f));

    return true;
}

bool FAllSettingsShouldExist::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    const UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    auto* OptionsMenuUserWidget = FindWidgetByClass<USHGOptionsMenuUserWidget>();
    if (!TestNotNull("OptionsMenuUserWidget must exists.", OptionsMenuUserWidget))
        return false;

    const auto* OptionsWidgetSwitcher = FindWidgetByName<UWidgetSwitcher>(OptionsMenuUserWidget, "OptionsWidgetSwitcher");
    if (!TestNotNull("OptionsWidgetSwitcher must exists.", OptionsWidgetSwitcher))
        return false;

    const auto* GameUserSettings = USHGGameUserSettings::Get();
    if (!TestNotNull("GameUserSettings should exists", GameUserSettings))
        return false;

    for (int32 i = 0; i != GameUserSettings->GetSettingsData().Num(); ++i)
    {
        auto* CurrentOptionsWidget = Cast<UUserWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(i));
        const auto* CurrentSettingsVerticalBox = FindWidgetByName<UVerticalBox>(CurrentOptionsWidget, "SettingsVerticalBox");
        if (!TestNotNull("CurrentSettingsVerticalBox should exists", CurrentSettingsVerticalBox))
            return false;

        TestTrueExpr(GameUserSettings->GetSettingsData()[i].Settings.Num() == CurrentSettingsVerticalBox->GetChildrenCount());
    }

    return true;
}

bool FComboBoxSettingCanBeApplied::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    auto* GameUserSettings = USHGGameUserSettings::Get();
    if (!TestNotNull("GameUserSettings should exists", GameUserSettings))
        return false;

    const int32 AntiAliasingSettingNumber = 6;
    const auto* AntiAliasingQualitySetting =
        Cast<USHGIntSetting>(GameUserSettings->GetSettingsData()[static_cast<int32>(ESettingType::Video)].Settings[AntiAliasingSettingNumber]);
    if (!TestNotNull("AntiAliasingQualitySetting should exists", AntiAliasingQualitySetting))
        return false;

    const int32 AntiAliasingQualityBefore = GameUserSettings->GetAntiAliasingQuality();
    TestTrueExpr(AntiAliasingQualitySetting->GetCurrentValue() == AntiAliasingQualityBefore);

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            SetComboBoxNextOption(ESettingType::Video, AntiAliasingSettingNumber);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            const int32 AntiAliasingQualityAfter = GameUserSettings->GetAntiAliasingQuality();
            TestTrueExpr(AntiAliasingQualityAfter != AntiAliasingQualityBefore);

            GameUserSettings->SetAntiAliasingQuality(AntiAliasingQualityBefore);
            GameUserSettings->ApplyNonResolutionSettings();
            return true;
        }));

    return true;
}

bool FSliderSettingCanBeApplied::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    auto* GameUserSettings = USHGGameUserSettings::Get();
    if (!TestNotNull("GameUserSettings should exists", GameUserSettings))
        return false;

    const int32 MouseXSensitivitySettingNumber = 1;
    const auto* MouseXSensitivitySetting =
        Cast<USHGFloatSetting>(GameUserSettings->GetSettingsData()[static_cast<int32>(ESettingType::Control)].Settings[MouseXSensitivitySettingNumber]);
    if (!TestNotNull("MouseXSensitivitySetting should exists", MouseXSensitivitySetting))
        return false;

    const float MouseXSensitivityBefore = GameUserSettings->GetSensitivitySettings().MouseXSens;
    TestTrueExpr(MouseXSensitivitySetting->GetCurrentValue() == MouseXSensitivityBefore);

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            const float NextValue = FMath::Fmod(MouseXSensitivityBefore + 0.1f, 1.0f);
            SetSliderNextValue(ESettingType::Control, MouseXSensitivitySettingNumber, NextValue);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            const float MouseXSensitivityAfter = GameUserSettings->GetSensitivitySettings().MouseXSens;
            TestTrueExpr(MouseXSensitivityAfter != MouseXSensitivityBefore);

            SetSliderNextValue(ESettingType::Control, MouseXSensitivitySettingNumber, MouseXSensitivityBefore);
            return true;
        }));

    return true;
}

bool FCheckBoxSettingCanBeApplied::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    auto* GameUserSettings = USHGGameUserSettings::Get();
    if (!TestNotNull("GameUserSettings should exists", GameUserSettings))
        return false;

    const int32 InvertYAxisSettingNumber = 0;
    const auto* InvertYAxisSetting =
        Cast<USHGBoolSetting>(GameUserSettings->GetSettingsData()[static_cast<int32>(ESettingType::Control)].Settings[InvertYAxisSettingNumber]);
    if (!TestNotNull("InvertYAxisSetting should exists", InvertYAxisSetting))
        return false;

    const int32 InvertYAxisBefore = FMath::Sign(GameUserSettings->GetSensitivitySettings().MouseYSens) > 0 ? 0 : 1;
    TestTrueExpr(InvertYAxisSetting->GetCurrentValue() == InvertYAxisBefore);

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            ToggleCheckBox(ESettingType::Control, InvertYAxisSettingNumber);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            const int32 InvertYAxisAfter = FMath::Sign(GameUserSettings->GetSensitivitySettings().MouseYSens) > 0 ? 0 : 1;
            TestTrueExpr(InvertYAxisAfter != InvertYAxisBefore);

            ToggleCheckBox(ESettingType::Control, InvertYAxisSettingNumber);
            return true;
        }));

    return true;
}

#endif    // WITH_AUTOMATION_TESTS
