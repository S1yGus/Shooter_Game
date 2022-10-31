// Shooter_Game, All rights reserved.

#include "Menu/SHGGameModeMenu.h"
#include "UI/SHGMenuHUD.h"
#include "Menu/ShooterMenuPlayerController.h"

ASHGGameModeMenu::ASHGGameModeMenu()
{
    PlayerControllerClass = AShooterMenuPlayerController::StaticClass();
    HUDClass = ASHGMenuHUD::StaticClass();
}

void ASHGGameModeMenu::StartPlay()
{
    Super::StartPlay();

    SetGameState(EGameState::MainMenu);
}
