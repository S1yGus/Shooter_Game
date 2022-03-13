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

    virtual void MakeMainShot() override;
    virtual void MakeAlternativeShot() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    int32 NumberOfBulletsInShot = 15;
};
