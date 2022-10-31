// Shooter_Game, All rights reserved.

#include "UI/SHGGameHUD.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGGameModeBase.h"

void ASHGGameHUD::BeginPlay()
{
    Super::BeginPlay();

    check(PlayerHUDWidgetClass);
    check(PlayerSpectatingWidgetClass);
    check(PauseWidgetClass);
    check(GameOverWidgetClass);
    check(PopUpHintWidgetClass);
}

void ASHGGameHUD::BackToRootMenu()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>();
    if (!GameMode)
        return;

    GameMode->SetGameState(EGameState::Pause);
}

void ASHGGameHUD::SetupWidgets()
{
    GameWidgets.Add(EGameState::Game, CreateWidget<USHGAnimatedUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(EGameState::Spectating, CreateWidget<USHGAnimatedUserWidget>(GetWorld(), PlayerSpectatingWidgetClass));
    GameWidgets.Add(EGameState::Pause, CreateWidget<USHGAnimatedUserWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(EGameState::GameOver, CreateWidget<USHGAnimatedUserWidget>(GetWorld(), GameOverWidgetClass));
    GameWidgets.Add(EGameState::PopUpHint, CreateWidget<USHGAnimatedUserWidget>(GetWorld(), PopUpHintWidgetClass));

    Super::SetupWidgets();
}
