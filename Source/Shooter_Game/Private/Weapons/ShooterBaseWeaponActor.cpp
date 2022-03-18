// Shooter_Game, All rights reserved.

#include "Weapons/ShooterBaseWeaponActor.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "Weapons/ShooterProjectileBaseActor.h"
#include "Weapons/ShooterShellBaseActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

    CurrentAmmo = DefaultAmmo;
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
        StopRecoilRecoverTimer();
        MakeRecoil();
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
    if (GetController()->IsPlayerController() && GetOwner()->FindComponentByClass<UShooterWeaponComponent>())
    {
        const auto ZoomingNow = GetOwner()->FindComponentByClass<UShooterWeaponComponent>()->IsZoomingNow();
        CurrentShotSpreadRange = ZoomingNow ? WeaponStatsData.AimedShotSpread : WeaponStatsData.ShotSpread;
    }

    float ConeHalfAngleRad = FMath::DegreesToRadians(CurrentShotSpreadRange.X / 2);
    const auto HealthComponent = GetOwner()->FindComponentByClass<UShooterHealthComponent>();
    if (HealthComponent)
    {
        const auto CurrentHealth = HealthComponent->GetHealth();
        const auto CurrentShotSpread = FMath::GetMappedRangeValueClamped(FVector2D(100.0f, 1.0f), CurrentShotSpreadRange, CurrentHealth);
        ConeHalfAngleRad = FMath::DegreesToRadians(CurrentShotSpread / 2);
    }

    return FMath::VRandCone(Direction, ConeHalfAngleRad);
}

bool AShooterBaseWeaponActor::CheckShotDirection(const FHitResult& HitResult) const
{
    const auto Radians = FMath::Acos(FVector::DotProduct(GetMuzzleQuaternion().GetForwardVector(), GetShotDirectionNormal(HitResult)));
    const float Degries = FMath::RadiansToDegrees(Radians);
    return Degries < MaxShotDirectionDegrees;
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

void AShooterBaseWeaponActor::MakeRecoil()
{
    if (!CalculateRecoil())
        return;

    GetWorldTimerManager().SetTimer(RecoilTimerHandle, this, &AShooterBaseWeaponActor::RecoilTimerTick, RecoilTimerRate, true);
}

void AShooterBaseWeaponActor::StopRecoilRecoverTimer()
{
    if (!GetWorldTimerManager().IsTimerActive(RecoilRecoveryTimerHandle))
        return;

    GetWorldTimerManager().ClearTimer(RecoilRecoveryTimerHandle);
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

bool AShooterBaseWeaponActor::CalculateRecoil()
{
    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return false;

    InitialControllerInputRotation = OwnerPawn->GetViewRotation();

    const auto NumberOfTicks = RecoilTime / RecoilTimerRate;
    CurrentPitchRecoil = FMath::RandRange(MinPitchRecoilMagnitude, MaxPitchRecoilMagnitude) / NumberOfTicks;
    CurrentYawRecoil = FMath::RandRange(MinYawRecoilMagnitude, MaxYawRecoilMagnitude) / NumberOfTicks;

    CurrentPitchRecoil /= OwnerPawn->GetController<APlayerController>()->InputPitchScale;
    CurrentYawRecoil /= OwnerPawn->GetController<APlayerController>()->InputYawScale;

    CurrentRecoveryPitchRecoil = CurrentPitchRecoil / -RecoilRecoverScale;
    CurrentRecoveryYawRecoil = CurrentYawRecoil / -RecoilRecoverScale;

    return true;
}

void AShooterBaseWeaponActor::RecoilTimerTick()
{
    GetOwner<APawn>()->AddControllerPitchInput(CurrentPitchRecoil);
    GetOwner<APawn>()->AddControllerYawInput(CurrentYawRecoil);

    CurrentRecoilTime += RecoilTimerRate;
    if (CurrentRecoilTime >= RecoilTime)
    {
        GetWorldTimerManager().ClearTimer(RecoilTimerHandle);
        CurrentRecoilTime = 0.0f;

        GetWorldTimerManager().SetTimer(RecoilRecoveryTimerHandle, this, &AShooterBaseWeaponActor::RecoilRecoveryTimerTick, RecoilTimerRate, true);
    }
}

void AShooterBaseWeaponActor::RecoilRecoveryTimerTick()
{
    const auto CurrentControllerInputRotation = GetOwner<APawn>()->GetViewRotation();
    const auto Delta = UKismetMathLibrary::NormalizedDeltaRotator(CurrentControllerInputRotation, InitialControllerInputRotation);
    if (Delta.Pitch >= MaxPitchRecoilMagnitude + RecoilRecoverPitchAdditionalOffset             //
        || FMath::Abs(Delta.Yaw) >= MaxYawRecoilMagnitude + RecoilRecoverYawAdditionalOffset    //
        || Delta.Pitch <= 0)
    {
        GetWorldTimerManager().ClearTimer(RecoilRecoveryTimerHandle);
        return;
    }

    GetOwner<APawn>()->AddControllerPitchInput(CurrentRecoveryPitchRecoil);
    GetOwner<APawn>()->AddControllerYawInput(CurrentRecoveryYawRecoil);
}
