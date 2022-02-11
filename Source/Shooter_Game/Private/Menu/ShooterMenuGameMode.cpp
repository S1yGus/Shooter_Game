// Shooter_Game, All rights reserved.


#include "Menu/ShooterMenuGameMode.h"
#include "Menu/UI/ShooterMenuHUD.h"
#include "Menu/ShooterMenuPlayerController.h"

AShooterMenuGameMode::AShooterMenuGameMode()
{
    PlayerControllerClass = AShooterMenuPlayerController::StaticClass();
    HUDClass = AShooterMenuHUD::StaticClass();
}