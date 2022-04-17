// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameModeBase.h"
#include "ShooterCoreTypes.h"
#include "ShooterMenuGameMode.generated.h"

UCLASS()
class SHOOTER_GAME_API AShooterMenuGameMode : public AShooterGameModeBase
{
    GENERATED_BODY()

public:
    AShooterMenuGameMode();

    virtual void StartPlay() override;

    void InMainMenu();
};
