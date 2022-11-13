// Shooter_Game, All rights reserved.

#include "Weapons/ShooterBaseWeaponActor.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "Weapons/Components/ShooterRecoilComponent.h"
#include "Weapons/ShooterProjectileBaseActor.h"
#include "Weapons/ShooterShellBaseActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SHGHealthComponent.h"
#include "Components/SHGStaminaComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/SpotLightComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterUtils.h"
#include "Sound/SoundCue.h"

constexpr static float MaxShotDirectionDegrees = 80.0f;
constexpr static float ShellDropDirectionHalfAngle = 30.0f;
constexpr static float ShellDropRandomRotationRange = 360.0f;

#define ECC_BulletTrace ECollisionChannel::ECC_GameTraceChannel2

AShooterBaseWeaponActor::AShooterBaseWeaponActor()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);

    FXComponent = CreateDefaultSubobject<UShooterWeaponFXComponent>("WeaonFXComponent");
    RecoilComponent = CreateDefaultSubobject<UShooterRecoilComponent>("RecoilComponent");

    SpotLight = CreateDefaultSubobject<USpotLightComponent>("SpotLightComponent");
    SpotLight->SetHiddenInGame(true);
    SpotLight->AttenuationRadius = 11000.0f;
    SpotLight->OuterConeAngle = 26.0f;
    SpotLight->SourceRadius = 1.0f;
    SpotLight->bUseTemperature = true;
    SpotLight->Temperature = 7500.0f;
    SpotLight->VolumetricScatteringIntensity = 2.5f;
    SpotLight->SetupAttachment(WeaponMesh);
}

void AShooterBaseWeaponActor::StartFire()
{
    AlternativeFireMode ? MakeAlternativeShot() : MakeMainShot();
}

void AShooterBaseWeaponActor::StopFire()
{
}

void AShooterBaseWeaponActor::SwitchFireMode()
{
    if (!HasAlternativeFireMode)
        return;

    StopFire();

    AlternativeFireMode = !AlternativeFireMode;
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), FXComponent->GetSwitchModeSound(), GetActorLocation());
}

void AShooterBaseWeaponActor::TurnOffFlashlight(bool State)
{
    SpotLight->SetHiddenInGame(State);

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), FXComponent->GetSwitchFlashlightSound(), GetActorLocation());
}

bool AShooterBaseWeaponActor::ReloadClip()
{
    if (CurrentAmmo.Clips == 0 && !CurrentAmmo.InfiniteClips || CurrentAmmo.BulletsInClip == DefaultAmmo.BulletsInClip)
        return false;

    CurrentAmmo.BulletsInClip = DefaultAmmo.BulletsInClip;
    if (!CurrentAmmo.InfiniteClips)
        --CurrentAmmo.Clips;

    return true;
}

bool AShooterBaseWeaponActor::IsAmmoEmpty() const
{
    return !CurrentAmmo.InfiniteClips && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool AShooterBaseWeaponActor::IsNumberOfClipsMax() const
{
    return CurrentAmmo.InfiniteClips || (!IsClipEmpty() && CurrentAmmo.Clips == DefaultAmmo.Clips);
}

bool AShooterBaseWeaponActor::TryToAddAmmo(int32 ClipsAmount)
{
    if (DefaultAmmo.InfiniteClips || IsNumberOfClipsMax() || ClipsAmount <= 0)
        return false;

    CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, IsClipEmpty() ? DefaultAmmo.Clips + 1 : DefaultAmmo.Clips);
    return true;
}

void AShooterBaseWeaponActor::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    check(FXComponent);
    check(SpotLight);

    CurrentAmmo = DefaultAmmo;

    SpotLight->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FlashlightSocketName);
}

void AShooterBaseWeaponActor::MakeMainShot()
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

    CalculateOneShot();
    DecreaseAmmo();

    SpawnBulletShell();
    MakeFX();
    if (GetController()->IsPlayerController())
    {
        RecoilComponent->StopRecoilRecoverTimer();
        RecoilComponent->MakeRecoil();
    }
}

void AShooterBaseWeaponActor::MakeAlternativeShot()
{
}

void AShooterBaseWeaponActor::CalculateOneShot()
{
    if (!ProjectileClass)
        return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    FVector EndPoint = TraceEnd;
    FHitResult HitResult;
    if (MakeTrace(HitResult, TraceStart, TraceEnd))
    {
        if (CheckShotDirection(HitResult))
        {
            EndPoint = HitResult.ImpactPoint;
        }
    }

    FTransform Transform(FRotator::ZeroRotator, GetMuzzleLocation());
    const auto Projectile = GetWorld()->SpawnActorDeferred<AShooterProjectileBaseActor>(ProjectileClass, Transform);
    if (!Projectile)
        return;

    auto Direction = (EndPoint - GetMuzzleLocation()).GetSafeNormal();
    Projectile->SetShotDirection(Direction);
    Projectile->SetOwner(this);
    const auto WeaponStatsData = AlternativeFireMode ? AlternativeWeaponStatsData : MainWeaponStatsData;
    const auto DamageAmount = FMath::RandRange(WeaponStatsData.MinDamage, WeaponStatsData.MaxDamage);
    Projectile->SetDamage(DamageAmount);

    Projectile->FinishSpawning(Transform);
}

void AShooterBaseWeaponActor::MakeFX()
{
    const auto MuzzleFX = AlternativeFireMode ? FXComponent->GetAlternativeMuzzleFX() : FXComponent->GetMainMuzzleFX();
    if (MuzzleFX)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleFX, WeaponMesh, MuzzleSocketName);
    }

    const auto FireSound = AlternativeFireMode ? FXComponent->GetAlternativeFireSound() : FXComponent->GetMainFireSound();
    if (FireSound)
    {
        UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }

    FXComponent->MakeCameraShake();
}

AController* AShooterBaseWeaponActor::GetController() const
{
    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return nullptr;

    return OwnerPawn->GetController();
}

FVector AShooterBaseWeaponActor::GetMuzzleLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

FQuat AShooterBaseWeaponActor::GetMuzzleQuaternion() const
{
    return WeaponMesh->GetSocketQuaternion(MuzzleSocketName);
}

bool AShooterBaseWeaponActor::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FRotator ViewRotation;
    if (!GetViewPoint(TraceStart, ViewRotation))
        return false;

    TraceEnd = TraceStart + GetShotDirection(ViewRotation.Vector()) * TraceMaxDistance;
    return true;
}

bool AShooterBaseWeaponActor::MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_BulletTrace, CollisionParams);
}

FVector AShooterBaseWeaponActor::GetShotDirection(const FVector& Direction) const
{
    const auto WeaponStatsData = AlternativeFireMode ? AlternativeWeaponStatsData : MainWeaponStatsData;

    auto CurrentShotSpreadRange = WeaponStatsData.AIShotSpread;
    if (GetController()->IsPlayerController() && GetOwner()->FindComponentByClass<USHGBaseWeaponComponent>())
    {
        const auto ZoomingNow = GetOwner()->FindComponentByClass<USHGBaseWeaponComponent>()->IsZoomingNow();
        CurrentShotSpreadRange = ZoomingNow ? WeaponStatsData.AimedShotSpread : WeaponStatsData.ShotSpread;
    }

    float PercentAmount = 0.0f;
    float MaxPercent = 0.0f;
    if (const auto StaminaComponent = GetOwner()->FindComponentByClass<USHGStaminaComponent>())
    {
        PercentAmount += StaminaComponent->GetStaminaPercent();
        MaxPercent += 1.0f;
    }
    if (const auto HealthComponent = GetOwner()->FindComponentByClass<USHGHealthComponent>())
    {
        PercentAmount += HealthComponent->GetHealthPercent();
        MaxPercent += 1.0f;
    }

    const auto CurrentShotSpread = FMath::GetMappedRangeValueClamped(FVector2D(MaxPercent, 0.0f), CurrentShotSpreadRange, PercentAmount);
    const auto ConeHalfAngleRad = FMath::DegreesToRadians(CurrentShotSpread / 2);

    return FMath::VRandCone(Direction, ConeHalfAngleRad);
}

bool AShooterBaseWeaponActor::CheckShotDirection(const FHitResult& HitResult) const
{
    const auto Radians = FMath::Acos(FVector::DotProduct(GetMuzzleQuaternion().GetForwardVector(), GetShotDirectionNormal(HitResult)));
    const float Degries = FMath::RadiansToDegrees(Radians);
    return Degries < MaxShotDirectionDegrees;
}

void AShooterBaseWeaponActor::DecreaseAmmo()
{
    --CurrentAmmo.BulletsInClip;

    OnFired.Broadcast();
}

void AShooterBaseWeaponActor::SpawnBulletShell()
{
    const auto Transform = FTransform(FRotator(0, FMath::RandRange(0.0f, ShellDropRandomRotationRange), 0), GetShellWindowLocation());
    const auto Shell = GetWorld()->SpawnActorDeferred<AShooterShellBaseActor>(BulletShellClass, Transform);
    if (!Shell)
        return;

    const auto Direction = FMath::VRandCone(GetShellWindowQuaternion().GetForwardVector(), FMath::DegreesToRadians(ShellDropDirectionHalfAngle));
    Shell->SetDropDirection(Direction);

    Shell->FinishSpawning(Transform);
}

FVector AShooterBaseWeaponActor::GetShellWindowLocation() const
{
    return WeaponMesh->GetSocketLocation(ShellWindowSocketName);
}

FQuat AShooterBaseWeaponActor::GetShellWindowQuaternion() const
{
    return WeaponMesh->GetSocketQuaternion(ShellWindowSocketName);
}

bool AShooterBaseWeaponActor::GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    if (!GetController())
        return false;

    GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

FVector AShooterBaseWeaponActor::GetShotDirectionNormal(const FHitResult& HitResult) const
{
    return (HitResult.ImpactPoint - GetMuzzleLocation()).GetSafeNormal();
}
