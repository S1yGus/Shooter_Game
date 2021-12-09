// Shooter_Game, All rights reserved.

#include "ShooterGameModeBase.h"
#include "Player/ShooterBaseCharacter.h"
#include "Player/ShooterPlayerController.h"

AShooterGameModeBase::AShooterGameModeBase()
{
    DefaultPawnClass = AShooterBaseCharacter::StaticClass();
    PlayerControllerClass = AShooterPlayerController::StaticClass();
}