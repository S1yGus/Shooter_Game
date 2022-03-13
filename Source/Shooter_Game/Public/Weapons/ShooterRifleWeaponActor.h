// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ShooterBaseWeaponActor.h"
#include "ShooterRifleWeaponActor.generated.h"

UCLASS()
class SHOOTER_GAME_API AShooterRifleWeaponActor : public AShooterBaseWeaponActor
{
    GENERATED_BODY()

public:
    AShooterRifleWeaponActor();

    virtual void StopFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.01", ClampMax = "100.0"))
    float TimeBetweenShots = 0.15f;

    virtual void MakeAlternativeShot() override;

private:
    FTimerHandle ShotTimerHandle;
};
