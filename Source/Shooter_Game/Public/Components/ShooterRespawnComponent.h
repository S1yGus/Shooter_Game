// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterRespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API UShooterRespawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UShooterRespawnComponent();

    void StartRespawn(int32 RespawnTime);
    bool GetCurrentRespawnTime(float& CurrentRespawnTime) const;

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle RespawnTimerHandle;

    void EndRespawn();
};
