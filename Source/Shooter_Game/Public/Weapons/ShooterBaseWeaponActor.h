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

UCLASS()
class SHOOTER_GAME_API AShooterBaseWeaponActor : public AActor
{
    GENERATED_BODY()

public:
    AShooterBaseWeaponActor();

    virtual void StartFire();
    virtual void StopFire();
    bool GetZoomFOV(float& ZoomFOV) const;

    bool ReloadClip();
    bool IsNumberOfClipsMax();
    bool TryToAddAmmo(int32 ClipsAmount);

    FName GetArmorySocketName() const { return WeaponArmorySocketName; }
    FWeaponUIData GetUIData() const { return UIData; }
    FAmmoData GetAmmoData() const { return CurrentAmmo; }
    EWeaponType GetWeaponType() const { return WeaponType; }

    FOnClipEmptySignature OnClipEmpty;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterWeaponFXComponent* FXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponArmorySocketName = "WeaponArmoryRifleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    float MinDamage = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    float MaxDamage = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0"))
    FVector2D ShotSpread = FVector2D(3.0f, 10.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Zoom")
    bool CanZoom = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Zoom", Meta = (EditCondition = "CanZoom"))
    float ZoomFOVAngle = 70.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    virtual void BeginPlay() override;
    virtual void MakeShot();
    virtual void CalculateOneShot();

    AController* GetController() const;
    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    bool MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
    FVector GetMuzzleLocation() const;
    FQuat GetMuzzleQuaternion() const;
    FVector GetShotDirection(const FVector& Direction) const;
    bool CheckShotDirection(const FHitResult& HitResult) const;
    void DealDamage(AActor* Actor);
    bool IsClipEmpty() const;
    bool IsAmmoEmpty() const;
    void DecreaseAmmo();
    void AmmoInfo();

private:
    FAmmoData CurrentAmmo;

    APawn* GetOwnerPawn() const;
    bool GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetShotDirectionNormal(const FHitResult& HitResult) const;
};