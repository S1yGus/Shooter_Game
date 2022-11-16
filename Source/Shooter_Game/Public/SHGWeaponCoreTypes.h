#pragma once

#include "SHGUICoreTypes.h"
#include "SHGWeaponCoreTypes.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Shotgun,
    Launcher,
    Rifle,
    Pistol,
    Max
};

class AShooterBaseWeaponActor;

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AShooterBaseWeaponActor> WeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    bool InfiniteClips = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo", Meta = (ClampMin = "1"))
    int32 BulletsInClip = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo", Meta = (EditCondition = "!InfiniteClips", ClampMin = "0"))
    int32 Clips = 0;
};

USTRUCT(BlueprintType)
struct FWeaponStatsData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    float MinDamage = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    float MaxDamage = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0"))
    FVector2D ShotSpread = FVector2D{3.0f, 10.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0"))
    FVector2D AimedShotSpread = FVector2D{1.0f, 1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0"))
    FVector2D AIShotSpread = FVector2D{3.0f, 10.0f};
};

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangedSignature, const FWeaponUIData&, const FAmmoData&);
DECLARE_MULTICAST_DELEGATE(FOnFiredSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoInfoChangedSignature, const FAmmoData&);
