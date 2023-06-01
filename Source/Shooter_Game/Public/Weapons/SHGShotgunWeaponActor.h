// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/SHGBaseWeaponActor.h"
#include "SHGShotgunWeaponActor.generated.h"

UCLASS(Abstract)
class SHOOTER_GAME_API ASHGShotgunWeaponActor : public ASHGBaseWeaponActor
{
    GENERATED_BODY()

public:
    ASHGShotgunWeaponActor();

    virtual void MakeMainShot() override;
    virtual void MakeAlternativeShot() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    int32 NumberOfBulletsInShot = 15;
};
