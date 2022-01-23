// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ShooterBaseWeaponActor.h"
#include "ShooterShotgunWeaponActor.generated.h"

UCLASS()
class SHOOTER_GAME_API AShooterShotgunWeaponActor : public AShooterBaseWeaponActor
{
    GENERATED_BODY()

public:
    AShooterShotgunWeaponActor();

    virtual void StartFire() override;
    virtual void MakeShot() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    int32 NumberOfBulletsInShot = 15;
};
