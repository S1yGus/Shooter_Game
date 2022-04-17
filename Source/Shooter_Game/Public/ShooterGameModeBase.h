// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameModeBase.generated.h"

UCLASS()
class SHOOTER_GAME_API AShooterGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    FOnGameStateChangedSignature OnGameStateChanged;
    void InOptions();

protected:
    EGameState CurrentGameState = EGameState::WaitingToStart;

    void SetGameState(EGameState NewGameState);
};