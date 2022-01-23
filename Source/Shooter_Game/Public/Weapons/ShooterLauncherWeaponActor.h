// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ShooterBaseWeaponActor.h"
#include "ShooterLauncherWeaponActor.generated.h"

class AShooterProjectileBaseActor;

UCLASS()
class SHOOTER_GAME_API AShooterLauncherWeaponActor : public AShooterBaseWeaponActor
{
    GENERATED_BODY()

public:
    AShooterLauncherWeaponActor();

    virtual void StartFire() override;

protected:
    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AShooterProjectileBaseActor> ProjectileClass;

    virtual void CalculateOneShot() override;
};
