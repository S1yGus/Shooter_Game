// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponFXComponent.generated.h"

class UNiagaraSystem;
class UParticleSystem;
class UPhysicalMaterial;
class UCameraShakeBase;
class USoundCue;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API UShooterWeaponFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UShooterWeaponFXComponent();

    void MakeImactFX(const FHitResult& HitResult);
    void MakeTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
    void MakeCameraShake();

    UParticleSystem* GetMuzzleFX() { return MuzzleFX; }
    USoundCue* GetFireSound() { return FireSound; }
    USoundCue* GetNoAmmoSound() { return NoAmmoSound; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FImpactFXData DefaultImpactFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TMap<UPhysicalMaterial*, FImpactFXData> ImpactFXDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName TraceEndVariableName = "TraceEnd";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UParticleSystem* MuzzleFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FireSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* NoAmmoSound;
};
