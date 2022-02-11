// Shooter_Game, All rights reserved.

#include "Menu/ShooterMenuPlayerController.h"

void AShooterMenuPlayerController::BeginPlay()
{
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}