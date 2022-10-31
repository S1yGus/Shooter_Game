// Shooter_Game, All rights reserved.

#include "UI/Menu/SHGPauseUserWidget.h"
#include "UI/Menu/SHGButtonUserWidget.h"
#include "SHGGameModeBase.h"
#include "SHGGameInstance.h"
#include "Player/ShooterPlayerController.h"

void USHGPauseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ResumeButton);
    check(OptionsButton);
    check(MenuButton);
    check(QuitButton);

    ResumeButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedResumeButton);
    OptionsButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedOptionsButton);
    MenuButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedMenuButton);
    QuitButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedQuitButton);

    if (const auto PC = GetOwningPlayer<AShooterPlayerController>())
    {
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEsc);
    }
}

void USHGPauseUserWidget::OnPressedEsc()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedResumeButton();
}

void USHGPauseUserWidget::OnClickedResumeButton()
{
    ChangeGameState(EGameState::Game);
}

void USHGPauseUserWidget::OnClickedOptionsButton()
{
    ChangeGameState(EGameState::Options);
}

void USHGPauseUserWidget::OnClickedMenuButton()
{
    ChangeGameState(EGameState::MainMenu);
}

void USHGPauseUserWidget::OnClickedQuitButton()
{
    ChangeGameState(EGameState::Quit);
}

void USHGPauseUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    const auto GameInstance = GetWorld()->GetGameInstance<USHGGameInstance>();
    const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>();
    if (!GameInstance || !GameMode)
        return;

    switch (GameStateToSet)
    {
        case EGameState::Game:
            GameMode->ClearPause();
            break;
        case EGameState::MainMenu:
            GameInstance->OpenMainMenu();
            break;
        case EGameState::Quit:
            GameInstance->QuitGame(GetOwningPlayer());
            break;
        default:
            GameMode->SetGameState(GameStateToSet);
            break;
    }
}
