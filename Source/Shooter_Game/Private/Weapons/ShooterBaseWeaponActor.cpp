// Shooter_Game, All rights reserved.

#include "Weapons/ShooterBaseWeaponActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShake.h"
#include "Player/ShooterBaseCharacter.h"
#include "Components/ShooterHealthComponent.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"
#include "ShooterUtils.h"

constexpr static float MaxShotDirectionDegrees = 80.0f;

AShooterBaseWeaponActor::AShooterBaseWeaponActor()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);

    FXComponent = CreateDefaultSubobject<UShooterWeaponFXComponent>("WeaonFXComponent");
}

void AShooterBaseWeaponActor::StartFire()
{
}

void AShooterBaseWeaponActor::StopFire()
{
}

bool AShooterBaseWeaponActor::GetZoomFOV(float& ZoomFOV) const
{
    if (CanZoom)
    {
        ZoomFOV = ZoomFOVAngle;
        return true;
    }

    return false;
}

void AShooterBaseWeaponActor::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    check(FXComponent);

    CurrentAmmo = DefaultAmmo;
}

void AShooterBaseWeaponActor::MakeShot()
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

    CalculateOneShot();

    DecreaseAmmo();

    FXComponent->MakeCameraShake();
    MakeMuzzleFX();
}

void AShooterBaseWeaponActor::CalculateOneShot()
{
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FVector TraceFXEnd = TraceEnd;
    FHitResult HitResult;
    if (MakeTrace(HitResult, TraceStart, TraceEnd))
    {
        if (CheckShotDirection(HitResult))
        {
            TraceFXEnd = HitResult.ImpactPoint;
            // DrawDebugLine(GetWorld(), GetMuzzleLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 2.0f);
            // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 12, FColor::Purple, false, 2.0f, 0, 2.0f);
            DealDamage(HitResult.GetActor());
        }
        else
        {
            FVector TraceEndLimited = GetMuzzleLocation() + GetShotDirection(GetMuzzleQuaternion().GetForwardVector()) * TraceMaxDistance;
            TraceFXEnd = TraceEndLimited;
            // DrawDebugLine(GetWorld(), GetMuzzleLocation(), TraceEndLimited, FColor::Red, false, 2.0f, 0, 2.0f);

            if (MakeTrace(HitResult, GetMuzzleLocation(), TraceEndLimited))
            {
                DealDamage(HitResult.GetActor());
            }
        }

        FXComponent->MakeImactFX(HitResult);
    }

    FXComponent->MakeTraceFX(GetMuzzleLocation(), TraceFXEnd);
}

void AShooterBaseWeaponActor::MakeMuzzleFX()
{
    const auto MuzzleFX = FXComponent->GetMuzzleFX();
    if (!MuzzleFX)
        return;

    UGameplayStatics::SpawnEmitterAttached(FXComponent->GetMuzzleFX(), WeaponMesh, MuzzleSocketName);
}

APawn* AShooterBaseWeaponActor::GetOwnerPawn() const
{
    return Cast<APawn>(GetOwner());
}

AController* AShooterBaseWeaponActor::GetController() const
{
    APawn* OwnerPawn = GetOwnerPawn();
    if (!OwnerPawn)
        return nullptr;

    return OwnerPawn->GetController();
}

bool AShooterBaseWeaponActor::GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    AController* Controller = GetController();
    if (!Controller)
        return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

bool AShooterBaseWeaponActor::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    TraceEnd = TraceStart + GetShotDirection(ViewRotation.Vector()) * TraceMaxDistance;
    return true;
}

bool AShooterBaseWeaponActor::MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_EngineTraceChannel2, CollisionParams);
}

FVector AShooterBaseWeaponActor::GetMuzzleLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

FQuat AShooterBaseWeaponActor::GetMuzzleQuaternion() const
{
    return WeaponMesh->GetSocketQuaternion(MuzzleSocketName);
}

FVector AShooterBaseWeaponActor::GetShotDirection(const FVector& Direction) const
{
    const auto CurrentShotSpreadRange = GetController()->IsPlayerController() ? ShotSpread : AIShotSpread;

    float ConeHalfAngleRad;
    const auto OwnerPawn = GetOwnerPawn();
    const auto HealthComponent = OwnerPawn->FindComponentByClass<UShooterHealthComponent>();
    if (HealthComponent)
    {
        const float CurrentHealth = HealthComponent->GetHealth();
        const float CurrentShotSpread = FMath::GetMappedRangeValueClamped(FVector2D(100.0f, 1.0f), CurrentShotSpreadRange, CurrentHealth);
        ConeHalfAngleRad = FMath::DegreesToRadians(CurrentShotSpread / 2);
    }
    else
    {
        ConeHalfAngleRad = FMath::DegreesToRadians(CurrentShotSpreadRange.X / 2);
    }

    return FMath::VRandCone(Direction, ConeHalfAngleRad);
}

FVector AShooterBaseWeaponActor::GetShotDirectionNormal(const FHitResult& HitResult) const
{
    return (HitResult.ImpactPoint - GetMuzzleLocation()).GetSafeNormal();
}

bool AShooterBaseWeaponActor::CheckShotDirection(const FHitResult& HitResult) const
{
    const float DotProduct = FVector::DotProduct(GetMuzzleQuaternion().GetForwardVector(), GetShotDirectionNormal(HitResult));
    const float Radians = FMath::Acos(DotProduct);
    const float Degries = FMath::RadiansToDegrees(Radians);
    return Degries < MaxShotDirectionDegrees;
}

void AShooterBaseWeaponActor::DealDamage(AActor* Actor)
{
    if (!Actor)
        return;

    float DamageAmount = FMath::FRandRange(MinDamage, MaxDamage);
    UGameplayStatics::ApplyDamage(Actor, DamageAmount, GetController(), this, nullptr);
}

bool AShooterBaseWeaponActor::IsClipEmpty() const
{
    return CurrentAmmo.BulletsInClip == 0;
}

bool AShooterBaseWeaponActor::IsAmmoEmpty() const
{
    return !CurrentAmmo.InfiniteClips && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

float AShooterBaseWeaponActor::GetOptimalAttackDistance() const
{
    return OptimalDistance;
}

float AShooterBaseWeaponActor::GetMaxAttackDistance() const
{
    return TraceMaxDistance;
}

void AShooterBaseWeaponActor::DecreaseAmmo()
{
    --CurrentAmmo.BulletsInClip;
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

bool AShooterBaseWeaponActor::IsNumberOfClipsMax() const
{
    return !IsClipEmpty() && CurrentAmmo.Clips == DefaultAmmo.Clips;
}

bool AShooterBaseWeaponActor::TryToAddAmmo(int32 ClipsAmount)
{
    if (DefaultAmmo.InfiniteClips || IsNumberOfClipsMax() || ClipsAmount <= 0)
        return false;

    if (IsClipEmpty())
    {
        CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, DefaultAmmo.Clips + 1);
    }
    else
    {
        CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, DefaultAmmo.Clips);
    }

    return true;
}

// Debug
void AShooterBaseWeaponActor::AmmoInfo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.BulletsInClip) + " / ";
    AmmoInfo += CurrentAmmo.InfiniteClips ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);

    UE_LOG(BaseWeaponLog, Display, TEXT("%s"), *AmmoInfo)
}
