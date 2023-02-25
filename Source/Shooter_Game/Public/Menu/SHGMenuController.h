// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SHGMenuController.generated.h"

UCLASS()
class SHOOTER_GAME_API ASHGMenuController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
};
