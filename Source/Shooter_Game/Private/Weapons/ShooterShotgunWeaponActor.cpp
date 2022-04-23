// Shooter_Game, All rights reserved.

#include "Weapons/ShooterShotgunWeaponActor.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "Weapons/Components/ShooterRecoilComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AShooterShotgunWeaponActor::AShooterShotgunWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryShotgunSocket";
    WeaponType = EWeaponType::Shotgun;
}

void AShooterShotgunWeaponActor::MakeMainShot()
{
    if (IsClipEmpty())
    {
        StopFire();

        if (IsAmmoEmpty())
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FXComponent->GetNoAmmoSound(), GetActorLocation());
        }
        else
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

    SpawnBulletShell();
    MakeFX();
    if (GetController()->IsPlayerController())
    {
        RecoilComponent->StopRecoilRecoverTimer();
        RecoilComponent->MakeRecoil();
    }
}

void AShooterShotgunWeaponActor::MakeAlternativeShot()
{
    MakeMainShot();
}
