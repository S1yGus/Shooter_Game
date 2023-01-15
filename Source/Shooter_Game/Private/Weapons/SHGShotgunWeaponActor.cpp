// Shooter_Game, All rights reserved.

#include "Weapons/SHGShotgunWeaponActor.h"
#include "Weapons/Components/SHGWeaponFXComponent.h"
#include "Weapons/Components/SHGRecoilComponent.h"

ASHGShotgunWeaponActor::ASHGShotgunWeaponActor() : ASHGBaseWeaponActor()
{
    WeaponData.WeaponArmorySocketName = "WeaponArmoryShotgunSocket";
    WeaponData.WeaponType = EWeaponType::Shotgun;
}

void ASHGShotgunWeaponActor::MakeMainShot()
{
    if (!CheckShot())
        return;

    for (int32 i = 0; i < NumberOfBulletsInShot; ++i)
    {
        FVector Direction;
        if (CalculateOneShot(Direction))
        {
            SpawnProjectile(Direction);
        }
    }

    FXComponent->MakeFireFX(WeaponMesh, WeaponData.MuzzleSocketName, bAlternativeFireMode);
    DecreaseAmmo();
    SpawnBulletShell();
    if (const auto Controller = GetController())
    {
        if (Controller->IsPlayerController())
        {
            RecoilComponent->StopRecoilRecoverTimer();
            RecoilComponent->MakeRecoil();
        }
    }

    OnFired.Broadcast();
}

void ASHGShotgunWeaponActor::MakeAlternativeShot()
{
    MakeMainShot();
}
