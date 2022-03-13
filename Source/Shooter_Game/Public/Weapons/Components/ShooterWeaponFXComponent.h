// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponFXComponent.generated.h"

class UPhysicalMaterial;
class USoundCue;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API UShooterWeaponFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UShooterWeaponFXComponent();

    void MakeImpactFX(const FHitResult& HitResult);
    void MakeTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
    void MakeCameraShake();

    UParticleSystem* GetMainMuzzleFX() const { return MainWeaponFXData.MuzzleFX; }
    USoundCue* GetMainFireSound() const { return MainWeaponFXData.FireSound; }

    UParticleSystem* GetAlternativeMuzzleFX() const { return AlternativeWeaponFXData.MuzzleFX; }
    USoundCue* GetAlternativeFireSound() const { return AlternativeWeaponFXData.FireSound; }

    USoundCue* GetNoAmmoSound() const { return NoAmmoSound; }
    USoundCue* GetSwitchModeSound() const { return SwitchModeSound; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FWeaponFXData MainWeaponFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FWeaponFXData AlternativeWeaponFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FName TraceEndVariableName = "TraceEnd";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* NoAmmoSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* SwitchModeSound;
};