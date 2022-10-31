// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterCoreTypes.h"
#include "SHGUICoreTypes.h"
#include "SHGGameModeBase.generated.h"

UCLASS(Abstract)
class SHOOTER_GAME_API ASHGGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    FOnGameStateChangedSignature OnGameStateChanged;

    void SetGameState(EGameState NewGameState);

protected:
    EGameState CurrentGameState = EGameState::WaitingToStart;
};