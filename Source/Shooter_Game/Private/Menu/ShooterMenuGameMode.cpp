// Shooter_Game, All rights reserved.

#include "Menu/ShooterMenuGameMode.h"
#include "UI/ShooterMenuHUD.h"
#include "Menu/ShooterMenuPlayerController.h"

AShooterMenuGameMode::AShooterMenuGameMode()
{
    PlayerControllerClass = AShooterMenuPlayerController::StaticClass();
    HUDClass = AShooterMenuHUD::StaticClass();
}

void AShooterMenuGameMode::StartPlay()
{
    Super::StartPlay();

    SetGameState(EGameState::MainMenu);
}

void AShooterMenuGameMode::InMainMenu()
{
    SetGameState(EGameState::MainMenu);
}
