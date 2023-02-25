// Shooter_Game, All rights reserved.

#include "Menu/SHGMenuController.h"

void ASHGMenuController::BeginPlay()
{
    SetInputMode(FInputModeUIOnly{});
    bShowMouseCursor = true;
}
