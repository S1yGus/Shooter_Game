// Shooter_Game, All rights reserved.

#include "UI/SHGMenuHUD.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGGameModeBase.h"

void ASHGMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    check(MainMenuWidgetClass);
}

void ASHGMenuHUD::BackToRootMenu()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>();
    if (!GameMode)
        return;

    GameMode->SetGameState(EGameState::MainMenu);
}

void ASHGMenuHUD::SetupWidgets()
{
    GameWidgets.Add(EGameState::MainMenu, CreateWidget<USHGAnimatedUserWidget>(GetWorld(), MainMenuWidgetClass));

    Super::SetupWidgets();
}
