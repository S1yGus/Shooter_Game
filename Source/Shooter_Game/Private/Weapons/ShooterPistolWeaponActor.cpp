// Shooter_Game, All rights reserved.


#include "Weapons/ShooterPistolWeaponActor.h"

AShooterPistolWeaponActor::AShooterPistolWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryPistolSocket";
}

void AShooterPistolWeaponActor::StartFire()
{
    MakeShot();
}