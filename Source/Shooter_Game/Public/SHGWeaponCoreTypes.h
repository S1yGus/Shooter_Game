#pragma once

#include "SHGUICoreTypes.h"
#include "SHGWeaponCoreTypes.generated.h"

class ASHGBaseProjectileActor;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Pistol,
    Shotgun,
    Rifle,
    Launcher,
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
    FName MuzzleSocketName{"MuzzleSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sockets")
    FName ShellWindowSocketName{"ShellWindowSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sockets")
    FName ModeSwitcherSocketName{"ModeSwitcherSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sockets")
    FName FlashlightSocketName{"FlashlightSocket"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    bool InfiniteClips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo", Meta = (ClampMin = "0"))
    int32 BulletsInClip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo", Meta = (EditCondition = "!InfiniteClips", ClampMin = "0"))
    int32 Clips;

    FString ToString() const
    {
        return FString::Printf(TEXT("(InfiniteClips=%i,BulletsInClip=%i,Clips=%i)"),    //
                               InfiniteClips,                                           //
                               BulletsInClip,                                           //
                               Clips);
    }
};

USTRUCT(BlueprintType)
struct FWeaponAIStats
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", Meta = (ClampMin = "0.0", Units = "cm"))
    float MinAttackDistance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", Meta = (ClampMin = "0.0", Units = "cm"))
    float MaxAttackDistance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", Meta = (ClampMin = "1"))
    int32 Priority = 1;
};

USTRUCT(BlueprintType)
struct FWeaponStats
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASHGBaseProjectileActor> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.01", Units = "s"))
    float TimeBetweenShots = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
    bool bCanZoom = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom", Meta = (EditCondition = "bCanZoom", Units = "deg"))
    float ZoomFOVAngle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0", Units = "deg"))
    FVector2D ShotSpread;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (EditCondition = "bCanZoom", ClampMin = "0.0", ClampMax = "90.0", Units = "deg"))
    FVector2D AimedShotSpread;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FWeaponAIStats WeaponAIStats;
};

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangedSignature, const FWeaponUIData&, const FAmmoData&);
DECLARE_MULTICAST_DELEGATE(FOnFiredSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoInfoChangedSignature, const FAmmoData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnZoomSignature, bool);
