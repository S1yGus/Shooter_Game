// Shooter_Game, All rights reserved.

#include "Player/SHGPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerSateLog, All, All)

void ASHGPlayerState::AddKill()
{
    OnPlayerStateChanged.Broadcast(++KillsNum, DeathsNum);
}

void ASHGPlayerState::AddDeath()
{
    OnPlayerStateChanged.Broadcast(KillsNum, ++DeathsNum);
}

void ASHGPlayerState::LogInfo()
{
    UE_LOG(PlayerSateLog, Display, TEXT("TeamID: %i Kills/Deaths: %i/%i "), TeamID, KillsNum, DeathsNum)
}
