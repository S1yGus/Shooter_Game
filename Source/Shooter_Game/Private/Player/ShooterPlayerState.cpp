// Shooter_Game, All rights reserved.

#include "Player/ShooterPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerSateLog, All, All)

void AShooterPlayerState::LogInfo()
{
    UE_LOG(PlayerSateLog, Display, TEXT("TeamID: %i Kills/Deaths: %i/%i "), TeamID, KillsNum, DeathsNum)
}
