#pragma once

#include "SHGUICoreTypes.h"
#include "SHGWeaponCoreTypes.generated.h"

class ASHGBaseProjectileActor;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Shotgun,
    Launcher,
    Rifle,
    Pistol,
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sockets")
    FName WeaponArmorySocketName{NAME_None};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sockets")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sockets")
    FName ShellWindowSocketName = "ShellWindowSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sockets")
    FName ModeSwitcherSocketName = "ModeSwitcherSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sockets")
    FName FlashlightSocketName = "FlashlightSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
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

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASHGBaseProjectileActor> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.01", ClampMax = "100.0"))
    float TimeBetweenShots = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0"))
    FVector2D ShotSpread{0.0f, 0.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0"))
    FVector2D AimedShotSpread{0.0f, 0.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", Meta = (ClampMin = "0"))
    float OptimalDistance = 0.0f;
};

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangedSignature, const FWeaponUIData&, const FAmmoData&);
DECLARE_MULTICAST_DELEGATE(FOnFiredSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoInfoChangedSignature, const FAmmoData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnZoomSignature, bool);
