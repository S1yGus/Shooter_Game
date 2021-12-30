// Shooter_Game, All rights reserved.


#include "Weapons/ShooterPistolWeaponActor.h"

AShooterPistolWeaponActor::AShooterPistolWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryPistolSocket";
    WeaponType = EWeaponType::Pistol;
}

void AShooterPistolWeaponActor::StartFire()
{
    MakeShot();
}