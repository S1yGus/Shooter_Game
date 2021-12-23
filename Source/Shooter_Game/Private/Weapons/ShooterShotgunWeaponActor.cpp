// Shooter_Game, All rights reserved.


#include "Weapons/ShooterShotgunWeaponActor.h"

AShooterShotgunWeaponActor::AShooterShotgunWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryShotgunSocket";
}

void AShooterShotgunWeaponActor::StartFire()
{
    MakeShot();
}

void AShooterShotgunWeaponActor::MakeShot()
{
    if (IsClipEmpty())
    {
        StopFire();

        if (!IsAmmoEmpty())
        {
            OnClipEmpty.Broadcast();
        }
        return;
    }

    for (int32 i = 0; i < NumberOfBulletsInShot; ++i)
    {
        CalculateOneShot();
    }

    DecreaseAmmo();
    MakeCameraShake();
    AmmoInfo();
}
