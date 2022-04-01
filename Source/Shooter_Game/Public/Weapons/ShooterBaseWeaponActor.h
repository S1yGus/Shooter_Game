// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterCoreTypes.h"
#include "ShooterBaseWeaponActor.generated.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All)

class USkeletalMeshComponent;
class AShooterBaseCharacter;
class UShooterWeaponFXComponent;
class AShooterProjectileBaseActor;
class AShooterShellBaseActor;
class USpotLightComponent;

UCLASS()
class SHOOTER_GAME_API AShooterBaseWeaponActor : public AActor
{
    GENERATED_BODY()

public:
    FOnClipEmptySignature OnClipEmpty;

    AShooterBaseWeaponActor();

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float MinPitchRecoilMagnitude = 2.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float MaxPitchRecoilMagnitude = 2.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float MinYawRecoilMagnitude = -0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float MaxYawRecoilMagnitude = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float RecoilTime = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float RecoilTimerRate = 0.007f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float RecoilRecoverScale = 4.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float RecoilRecoverPitchAdditionalOffset = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
    float RecoilRecoverYawAdditionalOffset = 2.0f;

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
    void DecreaseAmmo() { --CurrentAmmo.BulletsInClip; };

    void SpawnBulletShell();

    void MakeRecoil();
    void StopRecoilRecoverTimer();

private:
    FAmmoData CurrentAmmo;

    FTimerHandle RecoilTimerHandle;
    FTimerHandle RecoilRecoveryTimerHandle;
    FRotator InitialControllerInputRotation;
    float CurrentRecoilTime = 0.0f;
    float CurrentPitchRecoil = 0.0f;
    float CurrentYawRecoil = 0.0f;
    float CurrentRecoveryPitchRecoil = 0.0f;
    float CurrentRecoveryYawRecoil = 0.0f;

    FVector GetShellWindowLocation() const;
    FQuat GetShellWindowQuaternion() const;

    bool GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetShotDirectionNormal(const FHitResult& HitResult) const;

    bool CalculateRecoil();
    void RecoilTimerTick();
    void RecoilRecoveryTimerTick();
};