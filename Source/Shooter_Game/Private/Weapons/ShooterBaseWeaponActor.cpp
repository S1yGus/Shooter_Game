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
#include "..\..\Public\Weapons\ShooterBaseWeaponActor.h"

AShooterBaseWeaponActor::AShooterBaseWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void AShooterBaseWeaponActor::StartFire()
{
}

void AShooterBaseWeaponActor::StopFire()
{
}

void AShooterBaseWeaponActor::Zoom(bool condition)
{
    APlayerController* Controller = Cast<APlayerController>(GetController());
    if (!Controller || !Controller->PlayerCameraManager)
        return;

    if (condition)
    {
        DefaultFOVAngle = Controller->PlayerCameraManager->GetFOVAngle();
    }

    Controller->PlayerCameraManager->SetFOV(condition ? ZoomFOVAngle : DefaultFOVAngle);
}

void AShooterBaseWeaponActor::BeginPlay()
{
	Super::BeginPlay();

    check(WeaponMesh);
    check(CameraShakeClass);

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
    MakeCameraShake();
}

void AShooterBaseWeaponActor::CalculateOneShot()
{
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }
        
    FHitResult HitResult;
    if (MakeTrace(HitResult, TraceStart, TraceEnd))
    {
        if (CheckShotDirection(HitResult))
        {
            DrawDebugLine(GetWorld(), GetMuzzleLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 2.0f);
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 12, FColor::Purple, false, 2.0f, 0, 2.0f);
            DealDamage(HitResult.GetActor());
        }
        else
        {
            FVector TraceEndLimited =
                GetMuzzleLocation() + GetShotDirection(GetMuzzleQuaternion().GetForwardVector()) * TraceMaxDistance;
            DrawDebugLine(GetWorld(), GetMuzzleLocation(), TraceEndLimited, FColor::Red, false, 2.0f, 0, 2.0f);

            if (MakeTrace(HitResult, GetMuzzleLocation(), TraceEndLimited))
            {
                DealDamage(HitResult.GetActor());
            }
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleLocation(), TraceEnd, FColor::Red, false, 2.0f, 0, 2.0f);
    }
}

AShooterBaseCharacter* AShooterBaseWeaponActor::GetCharacter() const
{
    return Cast<AShooterBaseCharacter>(GetOwner());
}

AController* AShooterBaseWeaponActor::GetController() const
{
    AShooterBaseCharacter* Character = GetCharacter();
    if (!Character)
        return nullptr;

    return Character->GetController();
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
    return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
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
    const float CurrentHealth = GetCharacter()->GetHealthComponent()->GetHealth();
    const float CurrentShotSpread = FMath::GetMappedRangeValueClamped(FVector2D(100.0f, 1.0f), ShotSpread, CurrentHealth);
    float ConeHalfAngleRad = FMath::DegreesToRadians(CurrentShotSpread / 2);
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
    return Degries < 80;
}

void AShooterBaseWeaponActor::DealDamage(AActor* Actor)
{
    if (!Actor)
        return;

    float DamageAmount = FMath::FRandRange(MinDamage, MaxDamage);
    UGameplayStatics::ApplyDamage(Actor, DamageAmount, GetController(), this, nullptr);
}

void AShooterBaseWeaponActor::MakeCameraShake()
{
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
        return;

    if (!CameraShakeClass)
        return;

    const float CurrentHealth = GetCharacter()->GetHealthComponent()->GetHealth();
    const float Scale = FMath::GetMappedRangeValueClamped(FVector2D(100.0f, 1.0f), FVector2D(0.2, 1.0), CurrentHealth);
    PlayerController->PlayerCameraManager->StartMatineeCameraShake(CameraShakeClass, Scale);
}

bool AShooterBaseWeaponActor::IsClipEmpty() const
{
    return CurrentAmmo.BulletsInClip == 0;
}

bool AShooterBaseWeaponActor::IsAmmoEmpty() const
{
    return !CurrentAmmo.InfiniteClips && CurrentAmmo.Clips == 0 && IsClipEmpty();
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

bool AShooterBaseWeaponActor::IsNumberOfClipsMax()
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

void AShooterBaseWeaponActor::AmmoInfo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.BulletsInClip) + " / ";
    AmmoInfo += CurrentAmmo.InfiniteClips ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);

    UE_LOG(BaseWeaponLog, Display, TEXT("%s"), *AmmoInfo)
}
