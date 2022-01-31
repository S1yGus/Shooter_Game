// Shooter_Game, All rights reserved.

#include "Player/ShooterPlayerController.h"
#include "Components/ShooterRespawnComponent.h"

AShooterPlayerController::AShooterPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<UShooterRespawnComponent>("RespawnComponent");
}