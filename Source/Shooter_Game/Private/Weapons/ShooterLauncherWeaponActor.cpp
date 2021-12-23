// Shooter_Game, All rights reserved.


#include "Weapons/ShooterLauncherWeaponActor.h"
#include "Weapons/ShooterProjectileBaseActor.h"
#include "..\..\Public\Weapons\ShooterLauncherWeaponActor.h"

AShooterLauncherWeaponActor::AShooterLauncherWeaponActor() : AShooterBaseWeaponActor()
{
    WeaponArmorySocketName = "WeaponArmoryLauncherSocket";
}

void AShooterLauncherWeaponActor::StartFire()
{
    MakeShot();
}

void AShooterLauncherWeaponActor::CalculateOneShot()
{
    if (!ProjectileClass)
        return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    FVector EndPoint;
    FHitResult HitResult;
    if (MakeTrace(HitResult, TraceStart, TraceEnd))
    {
        EndPoint = CheckShotDirection(HitResult) ? HitResult.ImpactPoint : TraceEnd;
    }
    else
    {
        EndPoint = TraceEnd;
    }

    FTransform Transform(FRotator::ZeroRotator, GetMuzzleLocation());
    AShooterProjectileBaseActor* Projectile =
        GetWorld()->SpawnActorDeferred<AShooterProjectileBaseActor>(ProjectileClass, Transform);
    if (!Projectile)
        return;

    const FVector Direction((EndPoint - GetMuzzleLocation()).GetSafeNormal());
    Projectile->SetShotDirection(Direction);
    Projectile->SetOwner(GetOwner());
    Projectile->FinishSpawning(Transform);
}
