// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/SHGBaseWeaponActor.h"
#include "SHGPistolWeaponActor.generated.h"

UCLASS(Abstract)
class SHOOTER_GAME_API ASHGPistolWeaponActor : public ASHGBaseWeaponActor
{
    GENERATED_BODY()

public:
    ASHGPistolWeaponActor();
};
