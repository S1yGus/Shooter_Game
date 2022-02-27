// Shooter_Game, All rights reserved.

#include "Weapons/ShooterRifleWeaponActor.h"
#include "DrawDebugHelpers.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

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

    SetFXActive(false);
}

void AShooterRifleWeaponActor::BeginPlay()
{
    Super::BeginPlay();

    InitFX();
    SetFXActive(false);
}

void AShooterRifleWeaponActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    FireAudioComponent->Stop();
}

void AShooterRifleWeaponActor::MakeFX()
{
    SetFXActive(true);

    FXComponent->MakeCameraShake();
}

void AShooterRifleWeaponActor::InitFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = UGameplayStatics::SpawnEmitterAttached(FXComponent->GetMuzzleFX(), WeaponMesh, MuzzleSocketName);
    }

    if (!FireAudioComponent)
    {
        FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FXComponent->GetFireSound(), WeaponMesh, MuzzleSocketName);
    }
}

void AShooterRifleWeaponActor::SetFXActive(bool IsActive)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetVisibility(IsActive);
    }
    
    if (FireAudioComponent)
    {
        FireAudioComponent->SetPaused(!IsActive);
    }
}
