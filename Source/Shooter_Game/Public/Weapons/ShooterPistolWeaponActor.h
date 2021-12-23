// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ShooterBaseWeaponActor.h"
#include "ShooterPistolWeaponActor.generated.h"

UCLASS()
class SHOOTER_GAME_API AShooterPistolWeaponActor : public AShooterBaseWeaponActor
{
	GENERATED_BODY()
	
public:
    AShooterPistolWeaponActor();

    virtual void StartFire() override;
};
