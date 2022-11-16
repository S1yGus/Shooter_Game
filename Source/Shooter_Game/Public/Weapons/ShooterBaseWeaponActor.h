// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGWeaponCoreTypes.h"
#include "SHGUICoreTypes.h"
#include "ShooterBaseWeaponActor.generated.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All)

class USkeletalMeshComponent;
class AShooterBaseCharacter;
class UShooterWeaponFXComponent;
class UShooterRecoilComponent;
class AShooterProjectileBaseActor;
class AShooterShellBaseActor;
class USpotLightComponent;

UCLASS()
class SHOOTER_GAME_API AShooterBaseWeaponActor : public AActor
{
    GENERATED_BODY()

public:
    AShooterBaseWeaponActor();

    FOnClipEmptySignature OnClipEmpty;
    FOnFiredSignature OnFired;

    virtual void StartFire();
    virtual void StopFire();

    bool IsCanZoom() const { return CanZoom; };
    float GetZoomFOVAngle() const { return ZoomFOVAngle; }

    void SwitchFireMode();
    bool IsFireModeAlternative() const { return AlternativeFireMode; }

    void TurnOffFlashlight(bool State);

    bool ReloadClip();
    bool IsAmmoEmpty() const;
    bool IsNumberOfClipsMax() const;
    void RestoreMaxNumberOfClips() { CurrentAmmo.Clips = DefaultAmmo.Clips; };
    bool TryToAddAmmo(int32 ClipsAmount);

    FName GetArmorySocketName() const { return WeaponArmorySocketName; }
    FWeaponUIData GetUIData() const { return UIData; }
    FAmmoData GetAmmoData() const { return CurrentAmmo; }
    EWeaponType GetWeaponType() const { return WeaponType; }
    float GetOptimalAttackDistance() const { return OptimalDistance; };
    float GetMaxAttackDistance() const { return TraceMaxDistance; };

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterWeaponFXComponent* FXComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterRecoilComponent* RecoilComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USpotLightComponent* SpotLight;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName FlashlightSocketName = "FlashlightSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Weapon", Meta = (EditCondition = "WeaponType != EWeaponType::Pistol"))
    TSubclassOf<AShooterProjectileBaseActor> ProjectileClass;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Weapon", Meta = (EditCondition = "WeaponType != EWeaponType::Pistol"))
    TSubclassOf<AShooterShellBaseActor> BulletShellClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName ShellWindowSocketName = "ShellWindowSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponArmorySocketName = "WeaponArmoryRifleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    float OptimalDistance = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FWeaponStatsData MainWeaponStatsData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FWeaponStatsData AlternativeWeaponStatsData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool HasAlternativeFireMode = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
    bool CanZoom = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom", Meta = (EditCondition = "CanZoom"))
    float ZoomFOVAngle = 70.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    bool AlternativeFireMode = false;

    virtual void BeginPlay() override;

    virtual void MakeMainShot();
    virtual void MakeAlternativeShot();
    virtual void CalculateOneShot();

    virtual void MakeFX();

    AController* GetController() const;
    FVector GetMuzzleLocation() const;
    FQuat GetMuzzleQuaternion() const;
    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    bool MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
    FVector GetShotDirection(const FVector& Direction) const;
    bool CheckShotDirection(const FHitResult& HitResult) const;

    bool IsClipEmpty() const { return CurrentAmmo.BulletsInClip == 0; };
    void DecreaseAmmo();

    void SpawnBulletShell();

private:
    FAmmoData CurrentAmmo;

    FVector GetShellWindowLocation() const;
    FQuat GetShellWindowQuaternion() const;

    bool GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetShotDirectionNormal(const FHitResult& HitResult) const;
};