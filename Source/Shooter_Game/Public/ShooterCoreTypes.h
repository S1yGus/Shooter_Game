#pragma once

#include "ShooterCoreTypes.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature)

class AShooterBaseWeaponActor;

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AShooterBaseWeaponActor> WeaponClasse;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* RaloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo", Meta = (ClampMin = "1"))
    int32 BulletsInClip = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo",
              Meta = (EditCondition = "!InfiniteClips", ClampMin = "0"))
    int32 Clips = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    bool InfiniteClips = false;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrossHairIcon;
};

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Name);