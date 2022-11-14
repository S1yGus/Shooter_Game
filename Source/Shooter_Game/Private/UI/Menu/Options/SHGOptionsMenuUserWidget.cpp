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

    if (const auto GameUserSettings = USHGGameUserSettings::Get())
    {
        GameUserSettings->OnResolutionChanged.AddUObject(this, &ThisClass::OnResolutionChanged);
    }

    if (const auto PC = GetOwningPlayer<ASHGPlayerController>())
    {
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEscape);
    }

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    FillOptionsButtonsBox();
}

void USHGOptionsMenuUserWidget::BackToRootMenu()
{
    if (const auto PC = GetOwningPlayer())
    {
        if (const auto HUD = PC->GetHUD<ASHGBaseHUD>())
        {
            HUD->BackToRootMenu();
        }
    }
}

void USHGOptionsMenuUserWidget::FillOptionsButtonsBox()
{
    const auto GameUserSettings = USHGGameUserSettings::Get();
    if (!GameUserSettings || GameUserSettings->GetSettings().Num() == 0)
        return;

    OptionsButtonsBox->ClearChildren();
    OptionsWidgetSwitcher->ClearChildren();

    int32 WidgetID = 0;
    for (const auto& SettingsData : GameUserSettings->GetSettings())
    {
        const auto OptionsWidget = CreateWidget<USHGOptionsUserWidget>(GetWorld(), OptionsWidgetClass);
        check(OptionsWidget);
        OptionsWidget->Init(SettingsData.Settings);
        OptionsWidgetSwitcher->AddChild(OptionsWidget);

        const auto OptionsCategoryButton = CreateWidget<USHGOptionsButtonUserWidget>(GetWorld(), OptionsButtonClass);
        check(OptionsCategoryButton);
        OptionsCategoryButton->Init(UEnum::GetDisplayValueAsText(SettingsData.Type), WidgetID++);
        OptionsCategoryButton->OnClickedOptionsButton.AddUObject(this, &ThisClass::OnSelectedOptionsCategory);
        OptionsButtonsBox->AddChild(OptionsCategoryButton);
    }

    SelectOptionsCategory();
}

void USHGOptionsMenuUserWidget::SelectOptionsCategory()
{
    OptionsWidgetSwitcher->SetActiveWidgetIndex(WidgetIDToSet);

    for (const auto& Widget : OptionsButtonsBox->GetAllChildren())
    {
        const auto OptionsButton = Cast<USHGOptionsButtonUserWidget>(Widget);
        if (!OptionsButton)
            continue;

        OptionsButton->SetSelected(OptionsButton->GetWidgetID() == WidgetIDToSet);
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

    for (const auto Widget : OptionsWidgetSwitcher->GetAllChildren())
    {
        if (const auto OptionsWidget = Cast<USHGOptionsUserWidget>(Widget))
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
        const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>();
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
