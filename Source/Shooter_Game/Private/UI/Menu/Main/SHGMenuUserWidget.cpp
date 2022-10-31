// Shooter_Game, All rights reserved.

#include "UI/Menu/Main/SHGMenuUserWidget.h"
#include "UI/Menu/SHGButtonUserWidget.h"
#include "SHGGameInstance.h"
#include "SHGGameModeBase.h"

void USHGMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(LoadingAnimation);
    check(NewGameButton);
    check(OptionsButton);
    check(QuitButton);

    NewGameButton->OnClickedButton.AddUObject(this, &ThisClass::OnStartNewGame);
    OptionsButton->OnClickedButton.AddUObject(this, &ThisClass::OnOptions);
    QuitButton->OnClickedButton.AddUObject(this, &ThisClass::OnQuit);
}

void USHGMenuUserWidget::OnStartNewGame()
{
    if (const auto GameInstance = GetWorld()->GetGameInstance<USHGGameInstance>())
    {
        if (GameInstance->GetNewGameLevelData().LevelName.IsNone())
            return;

        PlayAnimation(LoadingAnimation);
    }
}

void USHGMenuUserWidget::OnOptions()
{
    ChangeGameState(EGameState::Options);
}

void USHGMenuUserWidget::OnQuit()
{
    ChangeGameState(EGameState::Quit);
}

void USHGMenuUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    const auto GameInstance = GetWorld()->GetGameInstance<USHGGameInstance>();
    const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>();
    if (!GameInstance || !GameMode)
        return;

    if (Animation == FadeoutAnimation)
    {
        switch (GameStateToSet)
        {
            case EGameState::Quit:
                GameInstance->QuitGame(GetOwningPlayer());
                break;
            default:
                GameMode->SetGameState(GameStateToSet);
                break;
        }
    }

    if (Animation == LoadingAnimation)
    {
        GameInstance->StartGame();
    }
}
