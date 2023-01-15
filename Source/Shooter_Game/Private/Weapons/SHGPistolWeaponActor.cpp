// Shooter_Game, All rights reserved.

#include "Weapons/SHGPistolWeaponActor.h"
#include "Weapons/Components/SHGWeaponFXComponent.h"

ASHGPistolWeaponActor::ASHGPistolWeaponActor() : ASHGBaseWeaponActor()
{
    WeaponData.WeaponArmorySocketName = "WeaponArmoryPistolSocket";
    WeaponData.WeaponType = EWeaponType::Pistol;
}
