#pragma once

#include "SHGFXCoreTypes.generated.h"

class USoundCue;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    TArray<UMaterialInterface*> Material;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FVector Size = FVector{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    float LifeTime = 60.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    float FadeOutTime = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    float FadeScreenSize = 0.01f;
};

USTRUCT(BlueprintType)
struct FImpactFXData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    UNiagaraSystem* ImpactNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    USoundCue* ImpactSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FDecalData ImpactDecalData;
};

USTRUCT(BlueprintType)
struct FExplosionCameraShakeData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    TSubclassOf<UCameraShakeBase> ExplosionCameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    float ExplosionCameraShakeRadius = 0.0f;
};

USTRUCT(BlueprintType)
struct FWeaponFXData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    UNiagaraSystem* MuzzleFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FireSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    float FireLoudness = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    TSubclassOf<UCameraShakeBase> FireCameraShakeClass;
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

USTRUCT(BlueprintType)
struct FFootstepsFXData
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
struct FFootstepNotifyData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    bool IsLeft = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    bool NoSound = false;
};
