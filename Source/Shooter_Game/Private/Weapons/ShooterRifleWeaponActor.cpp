// Shooter_Game, All rights reserved.

#include "Weapons/ShooterRifleWeaponActor.h"
#include "DrawDebugHelpers.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AShooterRifleWeaponActor::AShooterRifleWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryRifleSocket";
    WeaponType = EWeaponType::Rifle;
}

void AShooterRifleWeaponActor::StartFire()
{
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShooterRifleWeaponActor::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void AShooterRifleWeaponActor::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);

    StopMuzzleFX();
}

void AShooterRifleWeaponActor::MakeMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        const auto MuzzleFX = FXComponent->GetMuzzleFX();
        MuzzleFXComponent = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, WeaponMesh, MuzzleSocketName);
    }

    MuzzleFXComponent->SetVisibility(true, true);
}

void AShooterRifleWeaponActor::StopMuzzleFX()
{
    if (!MuzzleFXComponent)
        return;

    MuzzleFXComponent->SetVisibility(false, true);
}
