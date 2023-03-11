// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGWeaponCoreTypes.h"
#include "SHGUICoreTypes.h"
#include "SHGBaseWeaponActor.generated.h"

class USkeletalMeshComponent;
class USHGWeaponFXComponent;
class USHGRecoilComponent;
class ASHGBaseProjectileActor;
class ASHGShellBaseActor;
class ASHGFlashlightActor;

UCLASS()
class SHOOTER_GAME_API ASHGBaseWeaponActor : public AActor
{
    GENERATED_BODY()

public:
    ASHGBaseWeaponActor();

    FOnClipEmptySignature OnClipEmpty;
    FOnFiredSignature OnFired;

    virtual void StartFire();
    virtual void StopFire();

    bool CanZoom() const { return bAlternativeFireMode ? AlternativeWeaponStats.bCanZoom : MainWeaponStats.bCanZoom; }
    float GetZoomFOVAngle() const { return bAlternativeFireMode ? AlternativeWeaponStats.ZoomFOVAngle : MainWeaponStats.ZoomFOVAngle; }

    void SwitchFireMode();
    void SetFlashlight(bool bCondition);
    bool ReloadClip();

    bool IsAmmoEmpty() const;
    bool TryToAddAmmo(int32 ClipsAmount);

    const FWeaponData& GetWeaponData() const { return WeaponData; }
    const FWeaponUIData& GetUIData() const { return UIData; }
    const FAmmoData& GetAmmoData() const { return CurrentAmmo; }
    int32 GetMaxNumberOfClips() const { return DefaultAmmo.Clips; }
    float GetMinAttackDistance() const;
    float GetMaxAttackDistance() const;
    int32 GetPriority() const;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGWeaponFXComponent* FXComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGRecoilComponent* RecoilComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flashlight")
    TSubclassOf<ASHGFlashlightActor> FlashlightClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FWeaponData WeaponData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool bHasAlternativeFireMode = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FWeaponStats MainWeaponStats;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (EditCondition = "bHasAlternativeFireMode"))
    FWeaponStats AlternativeWeaponStats;

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASHGShellBaseActor> BulletShellClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0", Units = "cm"))
    float TraceMaxDistance = 20000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    bool bAlternativeFireMode = false;

    inline AController* GetController() const;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void MakeMainShot();
    virtual void MakeAlternativeShot();

    bool CheckShot();
    bool CalculateOneShot(FVector& Direction);

    void DecreaseAmmo();

    void SpawnProjectile(const FVector& Direction);
    void SpawnBulletShell();

private:
    UPROPERTY()
    ASHGFlashlightActor* Flashlight = nullptr;
    FAmmoData CurrentAmmo;

    FTimerHandle ShotDelayTimerHandle;

    bool bOnShotDelay = false;
    bool bIsZoomingNow = false;
    float Difficulty;

    inline FVector GetMuzzleLocation() const;
    inline FQuat GetMuzzleQuaternion() const;
    inline FVector GetShellWindowLocation() const;
    inline FQuat GetShellWindowQuaternion() const;
    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    FVector GetShotDirection(const FVector& Direction) const;
    bool CheckShotDirection(const FHitResult& HitResult) const;
    bool GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    inline FVector GetShotDirectionNormal(const FHitResult& HitResult) const;
    inline bool IsClipEmpty() const { return CurrentAmmo.BulletsInClip == 0; }
    bool IsNumberOfClipsMax() const;

    bool MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
    inline void SpawnAndAttachFlashlight();
    void StopZoom();

    void OnReleaseShotDelayFlag();
    void OnZoom(bool bCondition);
    void OnDifficultyChanged(float NewDifficulty);
};
