// Shooter_Game, All rights reserved.

#include "Weapons/SHGLauncherWeaponActor.h"

ASHGLauncherWeaponActor::ASHGLauncherWeaponActor() : ASHGBaseWeaponActor()
{
    WeaponData.WeaponArmorySocketName = "WeaponArmoryLauncherSocket";
    WeaponData.WeaponType = EWeaponType::Launcher;
}
