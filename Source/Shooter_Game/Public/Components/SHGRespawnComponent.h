// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHGRespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGRespawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USHGRespawnComponent();

    void StartRespawn(float RespawnTime);
    bool GetCurrentRespawnTime(float& CurrentRespawnTime) const;

private:
    FTimerHandle RespawnTimerHandle;

    void OnEndRespawn();
};
