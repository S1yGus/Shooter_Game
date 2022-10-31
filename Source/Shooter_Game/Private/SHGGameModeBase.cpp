// Shooter_Game, All rights reserved.

#include "SHGGameModeBase.h"

void ASHGGameModeBase::SetGameState(EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;
    OnGameStateChanged.Broadcast(NewGameState);
}
