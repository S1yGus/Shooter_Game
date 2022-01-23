// Shooter_Game, All rights reserved.

#include "Weapons/ShooterShotgunWeaponActor.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"

AShooterShotgunWeaponActor::AShooterShotgunWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryShotgunSocket";
    WeaponType = EWeaponType::Shotgun;
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

    FXComponent->MakeCameraShake();
    MakeMuzzleFX();
}
