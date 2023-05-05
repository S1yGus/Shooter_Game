// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/SHGOptionsMenuUserWidget.h"
#include "UI/Menu/Options/SHGOptionsButtonUserWidget.h"
#include "UI/Menu/Options/SHGOptionsUserWidget.h"
#include "UI/Menu/SHGButtonUserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "Settings/SHGGameUserSettings.h"
#include "SHGGameModeBase.h"
#include "SHGBaseHUD.h"
#include "Player/SHGPlayerController.h"

void USHGOptionsMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SwitchInAnimation);
    check(SwitchOutAnimation);
    check(OptionsButtonsBox);
    check(OptionsWidgetSwitcher);
    check(BackButton);

    BackButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedBackButton);

    if (auto* GameUserSettings = USHGGameUserSettings::Get())
    {
        GameUserSettings->OnResolutionChanged.AddUObject(this, &ThisClass::OnResolutionChanged);
    }

    if (auto* PC = GetOwningPlayer<ASHGPlayerController>())
    {
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEscape);
    }

    if (auto* GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    OptionsButtonsBox->ClearChildren();
    OptionsWidgetSwitcher->ClearChildren();

    FillWidget();
}

void USHGOptionsMenuUserWidget::BackToRootMenu()
{
    if (const APlayerController* PC = GetOwningPlayer())
    {
        if (auto* HUD = PC->GetHUD<ASHGBaseHUD>())
        {
            HUD->BackToRootMenu();
        }
    }
}

void USHGOptionsMenuUserWidget::FillWidget()
{
    const auto* GameUserSettings = USHGGameUserSettings::Get();
    if (!GameUserSettings || GameUserSettings->GetSettingsData().IsEmpty())
        return;

    for (const auto& [Type, Settings] : GameUserSettings->GetSettingsData())
    {
        FillButtonsBox(Type, GameUserSettings->GetSettingsData().Num());
        FillWidgetSwitcher(Settings);
    }

    SelectOptionsCategory();
}

void USHGOptionsMenuUserWidget::FillButtonsBox(ESettingType SettingType, int32 ButtonsAmount)
{
    auto* OptionsCategoryButton = CreateWidget<USHGOptionsButtonUserWidget>(GetWorld(), OptionsButtonClass);
    check(OptionsCategoryButton);
    static int32 WidgetID = 0;
    OptionsCategoryButton->Init(UEnum::GetDisplayValueAsText(SettingType), WidgetID++ % ButtonsAmount);
    OptionsCategoryButton->OnClickedOptionsButton.AddUObject(this, &ThisClass::OnSelectedOptionsCategory);
    OptionsButtonsBox->AddChild(OptionsCategoryButton);
}

void USHGOptionsMenuUserWidget::FillWidgetSwitcher(const TArray<USHGSetting*>& Settings)
{
    auto* OptionsWidget = CreateWidget<USHGOptionsUserWidget>(GetWorld(), OptionsWidgetClass);
    check(OptionsWidget);
    OptionsWidget->Init(Settings);
    OptionsWidgetSwitcher->AddChild(OptionsWidget);
}

void USHGOptionsMenuUserWidget::SelectOptionsCategory()
{
    OptionsWidgetSwitcher->SetActiveWidgetIndex(WidgetIDToSet);

    for (auto* Widget : OptionsButtonsBox->GetAllChildren())
    {
        if (auto* OptionsButton = Cast<USHGOptionsButtonUserWidget>(Widget))
        {
            OptionsButton->SetSelected(OptionsButton->GetWidgetID() == WidgetIDToSet);
        }
    }
}

void USHGOptionsMenuUserWidget::OnPressedEscape()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedBackButton();
}

void USHGOptionsMenuUserWidget::OnClickedBackButton()
{
    ChangeGameState(EGameState::BackToRootMenu);
}

void USHGOptionsMenuUserWidget::OnResolutionChanged()
{
    ChangeGameState(EGameState::OptionsWarning);
}

void USHGOptionsMenuUserWidget::OnSelectedOptionsCategory(int32 WidgetID)
{
    if (WidgetID == WidgetIDToSet || IsAnyAnimationPlaying())
        return;

    WidgetIDToSet = WidgetID;
    PlayAnimation(SwitchInAnimation);
}

void USHGOptionsMenuUserWidget::OnGameStateChanged(EGameState GameState)
{
    if (GameState != EGameState::Options)
        return;

    for (auto* Widget : OptionsWidgetSwitcher->GetAllChildren())
    {
        if (auto* OptionsWidget = Cast<USHGOptionsUserWidget>(Widget))
        {
            OptionsWidget->UpdateOptions();
        }
    }
}

void USHGOptionsMenuUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == FadeoutAnimation)
    {
        auto* GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>();
        if (!GameMode)
            return;

        switch (GameStateToSet)
        {
            case EGameState::BackToRootMenu:
                BackToRootMenu();
                break;
            default:
                GameMode->SetGameState(GameStateToSet);
                break;
        }
    }

    if (Animation == SwitchInAnimation)
    {
        SelectOptionsCategory();
        PlayAnimation(SwitchOutAnimation);
    }
}
