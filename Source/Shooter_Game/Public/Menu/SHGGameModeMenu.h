// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SHGGameModeBase.h"
#include "SHGGameModeMenu.generated.h"

UCLASS()
class SHOOTER_GAME_API ASHGGameModeMenu : public ASHGGameModeBase
{
    GENERATED_BODY()

public:
    ASHGGameModeMenu();

    virtual void StartPlay() override;
};
