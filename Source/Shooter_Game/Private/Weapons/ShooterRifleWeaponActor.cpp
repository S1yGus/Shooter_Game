// Shooter_Game, All rights reserved.

#include "Weapons/ShooterRifleWeaponActor.h"

AShooterRifleWeaponActor::AShooterRifleWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryRifleSocket";
    WeaponType = EWeaponType::Rifle;
}

void AShooterRifleWeaponActor::StopFire()
{
    if (AlternativeFireMode)
    {
        GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    }
    else
    {
        Super::StopFire();
    }
}

void AShooterRifleWeaponActor::MakeAlternativeShot()
{
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShooterRifleWeaponActor::MakeMainShot, TimeBetweenShots, true);

    MakeMainShot();
}
