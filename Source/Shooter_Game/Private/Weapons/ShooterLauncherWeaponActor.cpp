// Shooter_Game, All rights reserved.

#include "Weapons/ShooterLauncherWeaponActor.h"

AShooterLauncherWeaponActor::AShooterLauncherWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryLauncherSocket";
    WeaponType = EWeaponType::Launcher;
}
