// Shooter_Game, All rights reserved.

#include "ShooterGameModeBase.h"

void AShooterGameModeBase::InOptions()
{
    SetGameState(EGameState::Options);
}

void AShooterGameModeBase::SetGameState(EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;

    OnGameStateChanged.Broadcast(NewGameState);
}
