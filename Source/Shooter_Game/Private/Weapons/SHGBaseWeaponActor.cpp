// Shooter_Game, All rights reserved.

#include "Weapons/SHGBaseWeaponActor.h"
#include "Weapons/Components/SHGWeaponFXComponent.h"
#include "Weapons/Components/SHGRecoilComponent.h"
#include "Weapons/SHGFlashlightActor.h"
#include "Weapons/SHGBaseProjectileActor.h"
#include "Weapons/SHGShellBaseActor.h"
#include "Components/SHGHealthComponent.h"
#include "Components/SHGStaminaComponent.h"
#include "Components/SHGBaseWeaponComponent.h"
#include "Settings/SHGGameUserSettings.h"

#define ECC_BulletTrace ECollisionChannel::ECC_GameTraceChannel2

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All)

constexpr static float MaxShotDirectionDegrees = 80.0f;
constexpr static float ShellDropDirectionHalfAngle = 30.0f;
constexpr static float ShellDropRandomRotationRange = 360.0f;

ASHGBaseWeaponActor::ASHGBaseWeaponActor()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);

    FXComponent = CreateDefaultSubobject<USHGWeaponFXComponent>("WeaonFXComponent");
    RecoilComponent = CreateDefaultSubobject<USHGRecoilComponent>("RecoilComponent");
}

void ASHGBaseWeaponActor::StartFire()
{
    if (bOnShotDelay)
        return;

    bOnShotDelay = true;
    GetWorldTimerManager().SetTimer(ShotDelayTimerHandle,                  //
                                    this,                                  //
                                    &ThisClass::OnReleaseShotDelayFlag,    //
                                    bAlternativeFireMode ? AlternativeWeaponStats.TimeBetweenShots : MainWeaponStats.TimeBetweenShots);

    bAlternativeFireMode ? MakeAlternativeShot() : MakeMainShot();
}

void ASHGBaseWeaponActor::StopFire()
{
}

void ASHGBaseWeaponActor::SwitchFireMode()
{
    if (!bHasAlternativeFireMode)
        return;

    StopFire();
    StopZoom();

    bAlternativeFireMode = !bAlternativeFireMode;
    FXComponent->PlaySwitchModeSound(WeaponMesh, WeaponData.ModeSwitcherSocketName);
}

void ASHGBaseWeaponActor::SetFlashlight(bool bCondition)
{
    Flashlight->SetFlashlight(bCondition);
    FXComponent->PlaySwitchFlashlightSound(WeaponMesh, WeaponData.FlashlightSocketName);
}

bool ASHGBaseWeaponActor::ReloadClip()
{
    if ((CurrentAmmo.Clips == 0 && !CurrentAmmo.InfiniteClips) || CurrentAmmo.BulletsInClip == DefaultAmmo.BulletsInClip)
        return false;

    CurrentAmmo.BulletsInClip = DefaultAmmo.BulletsInClip;
    if (!CurrentAmmo.InfiniteClips)
    {
        --CurrentAmmo.Clips;
    }

    return true;
}

bool ASHGBaseWeaponActor::IsAmmoEmpty() const
{
    return !CurrentAmmo.InfiniteClips && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASHGBaseWeaponActor::TryToAddAmmo(int32 ClipsAmount)
{
    if (IsNumberOfClipsMax() || ClipsAmount <= 0)
        return false;

    CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, IsClipEmpty() ? DefaultAmmo.Clips + 1 : DefaultAmmo.Clips);
    return true;
}

float ASHGBaseWeaponActor::GetMinAttackDistance() const
{
    return bAlternativeFireMode ? AlternativeWeaponStats.WeaponAIStats.MinAttackDistance : MainWeaponStats.WeaponAIStats.MinAttackDistance;
}

float ASHGBaseWeaponActor::GetMaxAttackDistance() const
{
    return bAlternativeFireMode ? AlternativeWeaponStats.WeaponAIStats.MaxAttackDistance : MainWeaponStats.WeaponAIStats.MaxAttackDistance;
}

int32 ASHGBaseWeaponActor::GetPriority() const
{
    return bAlternativeFireMode ? AlternativeWeaponStats.WeaponAIStats.Priority : MainWeaponStats.WeaponAIStats.Priority;
}

AController* ASHGBaseWeaponActor::GetController() const
{
    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return nullptr;

    return OwnerPawn->GetController();
}

void ASHGBaseWeaponActor::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    check(FXComponent);
    check(RecoilComponent);
    check(MainWeaponStats.ProjectileClass);
    if (bHasAlternativeFireMode)
    {
        check(AlternativeWeaponStats.ProjectileClass);
    }
    check(FlashlightClass);
    check(WeaponData.ReloadAnimMontage);
    check(WeaponData.WeaponArmorySocketName != NAME_Name);
    check(UIData.CrossHairIcon);
    check(UIData.MainIcon);

    CurrentAmmo = DefaultAmmo;

    SpawnAndAttachFlashlight();

    if (Owner)
    {
        if (const auto WeaponComponent = Owner->FindComponentByClass<USHGBaseWeaponComponent>())
        {
            WeaponComponent->OnZoom.AddUObject(this, &ThisClass::OnZoom);
        }
    }

    if (const auto GameUserSettings = USHGGameUserSettings::Get())
    {
        Difficulty = GameUserSettings->GetDifficulty();
        GameUserSettings->OnDifficultyChanged.AddUObject(this, &ThisClass::OnDifficultyChanged);
    }
}

void ASHGBaseWeaponActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Flashlight->Destroy();
}

void ASHGBaseWeaponActor::MakeMainShot()
{
    if (!CheckShot())
        return;

    FVector Direction;
    if (CalculateOneShot(Direction))
    {
        SpawnProjectile(Direction);
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
}

void ASHGBaseWeaponActor::MakeAlternativeShot()
{
}

bool ASHGBaseWeaponActor::CheckShot()
{
    if (IsClipEmpty())
    {
        StopFire();

        if (IsAmmoEmpty())
        {
            FXComponent->PlayNoAmmoSound(WeaponMesh, WeaponData.MuzzleSocketName);
        }
        else
        {
            OnClipEmpty.Broadcast();
        }

        return false;
    }

    return true;
}

bool ASHGBaseWeaponActor::CalculateOneShot(FVector& Direction)
{
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return false;

    FVector EndPoint = TraceEnd;
    FHitResult HitResult;
    if (MakeTrace(HitResult, TraceStart, TraceEnd))
    {
        if (CheckShotDirection(HitResult))
        {
            EndPoint = HitResult.ImpactPoint;
        }
    }

    Direction = (EndPoint - GetMuzzleLocation()).GetSafeNormal();
    return true;
}

void ASHGBaseWeaponActor::DecreaseAmmo()
{
    --CurrentAmmo.BulletsInClip;
}

void ASHGBaseWeaponActor::SpawnProjectile(const FVector& Direction)
{
    const FTransform Transform{FRotator::ZeroRotator, GetMuzzleLocation()};
    auto Projectile = GetWorld()->SpawnActorDeferred<ASHGBaseProjectileActor>(bAlternativeFireMode ? AlternativeWeaponStats.ProjectileClass    //
                                                                                                   : MainWeaponStats.ProjectileClass,          //
                                                                              Transform);
    if (!Projectile)
        return;

    Projectile->SetShotDirection(Direction);
    Projectile->SetOwner(this);
    Projectile->FinishSpawning(Transform);
}

void ASHGBaseWeaponActor::SpawnBulletShell()
{
    const auto Transform = FTransform(FRotator(0.0, FMath::RandRange(0.0f, ShellDropRandomRotationRange), 0.0), GetShellWindowLocation());
    auto Shell = GetWorld()->SpawnActorDeferred<ASHGShellBaseActor>(BulletShellClass, Transform);
    if (!Shell)
        return;

    const auto Direction = FMath::VRandCone(GetShellWindowQuaternion().GetForwardVector(), FMath::DegreesToRadians(ShellDropDirectionHalfAngle));
    Shell->SetDropDirection(Direction);
    Shell->FinishSpawning(Transform);
}

FVector ASHGBaseWeaponActor::GetMuzzleLocation() const
{
    return WeaponMesh->GetSocketLocation(WeaponData.MuzzleSocketName);
}

FQuat ASHGBaseWeaponActor::GetMuzzleQuaternion() const
{
    return WeaponMesh->GetSocketQuaternion(WeaponData.MuzzleSocketName);
}

FVector ASHGBaseWeaponActor::GetShellWindowLocation() const
{
    return WeaponMesh->GetSocketLocation(WeaponData.ShellWindowSocketName);
}

FQuat ASHGBaseWeaponActor::GetShellWindowQuaternion() const
{
    return WeaponMesh->GetSocketQuaternion(WeaponData.ShellWindowSocketName);
}

bool ASHGBaseWeaponActor::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FRotator ViewRotation;
    if (!GetViewPoint(TraceStart, ViewRotation))
        return false;

    TraceEnd = TraceStart + GetShotDirection(ViewRotation.Vector()) * TraceMaxDistance;
    return true;
}

FVector ASHGBaseWeaponActor::GetShotDirection(const FVector& Direction) const
{
    const auto Controller = GetController();
    if (!Controller || !Owner)
        return FVector::Zero();

    auto ShotSpread = bAlternativeFireMode ? AlternativeWeaponStats.ShotSpread : MainWeaponStats.ShotSpread;

    if (bIsZoomingNow)
    {
        ShotSpread = bAlternativeFireMode ? AlternativeWeaponStats.AimedShotSpread : MainWeaponStats.AimedShotSpread;
    }

    if (!Controller->IsPlayerController())
    {
        ShotSpread *= Difficulty;
    }

    float PercentAmount = 0.0f;
    float MaxPercent = 0.0f;
    if (const auto StaminaComponent = Owner->FindComponentByClass<USHGStaminaComponent>())
    {
        PercentAmount += StaminaComponent->GetStaminaPercent();
        MaxPercent += 1.0f;    // 1.0f for 100%.
    }

    if (const auto HealthComponent = Owner->FindComponentByClass<USHGHealthComponent>())
    {
        PercentAmount += HealthComponent->GetHealthPercent();
        MaxPercent += 1.0f;    // 1.0f for 100%.
    }

    const auto CurrentShotSpread = FMath::GetMappedRangeValueClamped(FVector2D(MaxPercent, 0.0f), ShotSpread, PercentAmount);
    const auto ConeHalfAngleRad = FMath::DegreesToRadians(CurrentShotSpread / 2);

    return FMath::VRandCone(Direction, ConeHalfAngleRad);
}

bool ASHGBaseWeaponActor::CheckShotDirection(const FHitResult& HitResult) const
{
    const auto Radians = FMath::Acos(FVector::DotProduct(GetMuzzleQuaternion().GetForwardVector(), GetShotDirectionNormal(HitResult)));
    const auto Degries = FMath::RadiansToDegrees(Radians);
    return Degries < MaxShotDirectionDegrees;
}

bool ASHGBaseWeaponActor::GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Contrioller = GetController();
    if (!Contrioller)
        return false;

    Contrioller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

FVector ASHGBaseWeaponActor::GetShotDirectionNormal(const FHitResult& HitResult) const
{
    return (HitResult.ImpactPoint - GetMuzzleLocation()).GetSafeNormal();
}

bool ASHGBaseWeaponActor::IsNumberOfClipsMax() const
{
    return CurrentAmmo.InfiniteClips || (!IsClipEmpty() && CurrentAmmo.Clips == DefaultAmmo.Clips);
}

bool ASHGBaseWeaponActor::MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;
    CollisionParams.bTraceComplex = true;

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_BulletTrace, CollisionParams);
}

void ASHGBaseWeaponActor::SpawnAndAttachFlashlight()
{
    Flashlight = GetWorld()->SpawnActor<ASHGFlashlightActor>(FlashlightClass);
    check(Flashlight);
    Flashlight->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponData.FlashlightSocketName);
}

void ASHGBaseWeaponActor::StopZoom()
{
    if (Owner)
    {
        if (const auto WeaponComponent = Owner->FindComponentByClass<USHGBaseWeaponComponent>())
        {
            WeaponComponent->Zoom(false);
        }
    }
}

void ASHGBaseWeaponActor::OnReleaseShotDelayFlag()
{
    bOnShotDelay = false;
}

void ASHGBaseWeaponActor::OnZoom(bool bCondition)
{
    bIsZoomingNow = bCondition;
}

void ASHGBaseWeaponActor::OnDifficultyChanged(float NewDifficulty)
{
    Difficulty = NewDifficulty;
}
