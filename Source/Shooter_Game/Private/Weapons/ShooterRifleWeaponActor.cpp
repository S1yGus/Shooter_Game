// Shooter_Game, All rights reserved.


#include "Weapons/ShooterRifleWeaponActor.h"
#include "DrawDebugHelpers.h"

AShooterRifleWeaponActor::AShooterRifleWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryRifleSocket";
    WeaponType = EWeaponType::Rifle;
}

void AShooterRifleWeaponActor::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShooterRifleWeaponActor::MakeShot, TimeBetweenShots, true);
}

void AShooterRifleWeaponActor::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}
