// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

UCLASS()
class SHOOTER_GAME_API AShooterPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    int32 GetTeamID() const { return TeamID; }
    void SetTeamID(int32 NewTeamID) { TeamID = NewTeamID; }

    int32 GetKillsNum() const { return KillsNum; }
    void AddKill() { ++KillsNum; }
    int32 GetDeathsNum() const { return DeathsNum; }
    void AddDeath() { ++DeathsNum; }
    void LogInfo();

private:
    int32 TeamID = 0;

    int32 KillsNum = 0;
    int32 DeathsNum = 0;
};
