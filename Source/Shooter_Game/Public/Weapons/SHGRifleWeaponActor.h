// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/SHGBaseWeaponActor.h"
#include "SHGRifleWeaponActor.generated.h"

UCLASS()
class SHOOTER_GAME_API ASHGRifleWeaponActor : public ASHGBaseWeaponActor
{
    GENERATED_BODY()

public:
    ASHGRifleWeaponActor();

    virtual void StopFire() override;

protected:
    virtual void MakeAlternativeShot() override;

private:
    FTimerHandle ShotTimerHandle;
};
