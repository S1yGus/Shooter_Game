// Shooter_Game, All rights reserved.

#include "Menu/SHGGameModeMenu.h"
#include "UI/SHGMenuHUD.h"
#include "Menu/SHGMenuController.h"

ASHGGameModeMenu::ASHGGameModeMenu()
{
    PlayerControllerClass = ASHGMenuController::StaticClass();
    HUDClass = ASHGMenuHUD::StaticClass();
}

void ASHGGameModeMenu::StartPlay()
{
    Super::StartPlay();

    SetGameState(EGameState::MainMenu);
}
