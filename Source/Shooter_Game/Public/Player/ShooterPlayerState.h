// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterCoreTypes.h"
#include "ShooterPlayerState.generated.h"

UCLASS()
class SHOOTER_GAME_API AShooterPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    FOnPlayerStateChangedSignature OnPlayerStateChanged;

    int32 GetTeamID() const { return TeamID; }
    void SetTeamID(int32 NewTeamID) { TeamID = NewTeamID; }
    FLinearColor GetTeamColor() const { return TeamColor; }
    void SetTeamColor(const FLinearColor& NewTeamColor) { TeamColor = NewTeamColor; }

    int32 GetKillsNum() const { return KillsNum; }
    void AddKill();
    int32 GetDeathsNum() const { return DeathsNum; }
    void AddDeath();
    void LogInfo();

private:
    int32 TeamID = 0;
    FLinearColor TeamColor;

    int32 KillsNum = 0;
    int32 DeathsNum = 0;
};
