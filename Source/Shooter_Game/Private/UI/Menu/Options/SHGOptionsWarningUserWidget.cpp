// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/SHGOptionsWarningUserWidget.h"
#include "UI/Menu/SHGButtonUserWidget.h"
#include "SHGGameModeBase.h"
#include "Settings/SHGGameUserSettings.h"
#include "Player/SHGPlayerController.h"

FText USHGOptionsWarningUserWidget::GetCountdownTime() const
{
    return FText::AsNumber(static_cast<int32>(CountdownTime));
}

void USHGOptionsWarningUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SaveButton);
    check(CancelButton);

    SaveButton->OnClickedButton.AddUObject(this, &ThisClass::OnSaveSettings);
    CancelButton->OnClickedButton.AddUObject(this, &ThisClass::OnCancelSettings);

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }

    if (const auto PC = GetOwningPlayer<ASHGPlayerController>())
    {
        PC->OnPressedEnt.AddUObject(this, &ThisClass::OnPressedEnter);
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEsc);
    }
}

void USHGOptionsWarningUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    CountdownTime -= InDeltaTime;
    if (CountdownTime <= 0.0f && !IsAnimationPlaying(FadeoutAnimation))
    {
        OnCancelSettings();
    }
}

void USHGOptionsWarningUserWidget::ResetWidget()
{
    CountdownTime = CancelSettingsTime;
}

void USHGOptionsWarningUserWidget::OnGameStateChanged(EGameState NewGameState)
{
    if (NewGameState != EGameState::OptionsWarning)
        return;

    ResetWidget();
}

void USHGOptionsWarningUserWidget::OnPressedEnter()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnSaveSettings();
}

void USHGOptionsWarningUserWidget::OnPressedEsc()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnCancelSettings();
}

void USHGOptionsWarningUserWidget::OnSaveSettings()
{
    if (const auto GameUserSettings = USHGGameUserSettings::Get())
    {
        GameUserSettings->ConfirmVideoMode();
        ShowFadeoutAnimation();
    }
}

void USHGOptionsWarningUserWidget::OnCancelSettings()
{
    if (const auto GameUserSettings = USHGGameUserSettings::Get())
    {
        GameUserSettings->SetLastConfirmedResolutionSettings();
        ShowFadeoutAnimation();
    }
}

void USHGOptionsWarningUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
    {
        GameMode->SetGameState(EGameState::Options);
    }
}
