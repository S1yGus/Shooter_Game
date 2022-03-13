// Shooter_Game, All rights reserved.

#include "Weapons/ShooterPistolWeaponActor.h"
#include "Weapons/Components/ShooterWeaponFXComponent.h"

AShooterPistolWeaponActor::AShooterPistolWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryPistolSocket";
    WeaponType = EWeaponType::Pistol;
}

void AShooterPistolWeaponActor::CalculateOneShot()
{
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    FVector TraceFXEnd = TraceEnd;
    FHitResult HitResult;
    if (MakeTrace(HitResult, TraceStart, TraceEnd))
    {
        if (CheckShotDirection(HitResult))
        {
            // DrawDebugLine(GetWorld(), GetMuzzleLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 2.0f);
            // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 12, FColor::Purple, false, 2.0f, 0, 2.0f);
            DealDamage(HitResult);
            FXComponent->MakeImpactFX(HitResult);
            TraceFXEnd = HitResult.ImpactPoint;
        }
        else
        {
            FVector TraceEndLimited = GetMuzzleLocation() + GetShotDirection(GetMuzzleQuaternion().GetForwardVector()) * TraceMaxDistance;
            TraceFXEnd = TraceEndLimited;
            // DrawDebugLine(GetWorld(), GetMuzzleLocation(), TraceEndLimited, FColor::Red, false, 2.0f, 0, 2.0f);

            if (MakeTrace(HitResult, GetMuzzleLocation(), TraceEndLimited))
            {
                DealDamage(HitResult);
                FXComponent->MakeImpactFX(HitResult);
            }
        } 
    }
    else
    {
        // DrawDebugLine(GetWorld(), GetMuzzleLocation(), TraceEnd, FColor::Blue, false, 2.0f, 0, 2.0f);
    }

    FXComponent->MakeTraceFX(GetMuzzleLocation(), TraceFXEnd);
}

void AShooterPistolWeaponActor::DealDamage(const FHitResult& HitResult)
{
    if (!HitResult.GetActor())
        return;

    const auto WeaponStatsData = AlternativeFireMode ? AlternativeWeaponStatsData : MainWeaponStatsData;
    const auto DamageAmount = FMath::FRandRange(WeaponStatsData.MinDamage, WeaponStatsData.MaxDamage);

    FPointDamageEvent DamageEvent;
    DamageEvent.HitInfo = HitResult;
    HitResult.GetActor()->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
}