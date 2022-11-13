#pragma once

#include "ShooterCoreTypes.generated.h"

// Weapon
DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangedSignature, const FWeaponUIData&, const FAmmoData&);
DECLARE_MULTICAST_DELEGATE(FOnFiredSignature)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoInfoChangedSignature, const FAmmoData&);

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo", Meta = (ClampMin = "1"))
    int32 BulletsInClip = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo", Meta = (EditCondition = "!InfiniteClips", ClampMin = "0"))
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

USTRUCT(BlueprintType)
struct FWeaponStatsData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    float MinDamage = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0"))
    float MaxDamage = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0"))
    FVector2D ShotSpread = FVector2D(3.0f, 10.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0"))
    FVector2D AimedShotSpread = FVector2D(1.0f, 1.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", Meta = (ClampMin = "0.0", ClampMax = "90.0"))
    FVector2D AIShotSpread = FVector2D(3.0f, 10.0f);
};

// Health
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, AController*, AController*);    // AController* KillerController, AController* VictimController
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);          // float HealthAmount, float HealthPercent

// Stamina
DECLARE_MULTICAST_DELEGATE(FOnOutOfStaminaSignature);
DECLARE_MULTICAST_DELEGATE(FOnNotEnoughStaminaSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedSignature, float, float);    // float StaminaAmount, float StaminaPercent

// VFX
UENUM(BlueprintType)
enum class ELocomotionType : uint8
{
    Run,
    StartJump,
    EndJump
};

USTRUCT(BlueprintType)
struct FDecalData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TArray<UMaterialInterface*> Material;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FVector Size = FVector(9.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float LifeTime = 120.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float FadeOutTime = 10.0f;
};

class USoundCue;

USTRUCT(BlueprintType)
struct FImpactFXData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    UParticleSystem* ImpactParticleSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    USoundCue* ImpactSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FDecalData ImpactDecalData;
};

USTRUCT(BlueprintType)
struct FFootstepDecalDataPair
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FDecalData Right;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FDecalData Left;
};

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FFootstepFXData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    UNiagaraSystem* FootstepNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    USoundCue* FootstepSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FFootstepDecalDataPair FootstepDecalDataPair;
};

USTRUCT(BlueprintType)
struct FFootstepFXDataMap
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    TMap<UPhysicalMaterial*, FFootstepFXData> FootstepFXDataMap;
};

USTRUCT(BlueprintType)
struct FFootstepNotifyData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    ELocomotionType LocomotionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    bool IsLeft = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    bool NoSound = false;
};

USTRUCT(BlueprintType)
struct FWeaponFXData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FImpactFXData DefaultImpactFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TMap<UPhysicalMaterial*, FImpactFXData> ImpactFXDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UParticleSystem* MuzzleFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FireSound;
};

// Game
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerStateChangedSignature, int32, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNewRaundSignature, int32, int32);

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1", ClampMax = "100"))
    int32 PlayersNum = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1", ClampMax = "4"))
    int32 TeamsNum = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FLinearColor DefaultTeamColor = FLinearColor::Gray;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TArray<FLinearColor> TeamsColors = {FLinearColor::Red, FLinearColor::Blue, FLinearColor::Yellow, FLinearColor::Green};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1", ClampMax = "10"))
    int32 RaundsNum = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1", ClampMax = "600"))
    int32 RaundTime = 300;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (ClampMin = "1", ClampMax = "60"))
    int32 RespawnTime = 5;
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
    WaitingToStart,
    MainMenu,
    Options,
    OptionsWarning,
    PopUpHint,
    Game,
    Spectating,
    Pause,
    BackToRootMenu,
    GameOver,
    Quit
};
