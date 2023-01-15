// Shooter_Game, All rights reserved.

#include "Weapons/SHGRifleWeaponActor.h"

ASHGRifleWeaponActor::ASHGRifleWeaponActor() : ASHGBaseWeaponActor()
{
    WeaponData.WeaponArmorySocketName = "WeaponArmoryRifleSocket";
    WeaponData.WeaponType = EWeaponType::Rifle;
    bHasAlternativeFireMode = true;
    bAlternativeFireMode = true;
}

void ASHGRifleWeaponActor::StopFire()
{
    if (bAlternativeFireMode)
    {
        GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    }
    else
    {
        Super::StopFire();
    }
}

void ASHGRifleWeaponActor::MakeAlternativeShot()
{
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ThisClass::MakeMainShot, CurrentWeaponStatsData.TimeBetweenShots, true);

    MakeMainShot();
}
