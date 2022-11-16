// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHGFXCoreTypes.h"
#include "ShooterBaseVFXComponent.generated.h"

class USoundCue;
class ASHGImpactIndicatorActor;
class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGBaseVFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USHGBaseVFXComponent();

    void MakeFootstepVFX(const FFootstepNotifyData& FootstepNotifyData);
    void SpawnImpactIndicator(float DamageAmount, const FVector& HitLocation, UPhysicalMaterial* PhysicalMaterial);
    void PlayDeathSound();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    bool ShowImpactIndicators = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX", Meta = (EditCondition = "ShowImpactIndicators"))
    TSubclassOf<ASHGImpactIndicatorActor> ImpactIndicatorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|Footsteps")
    FName RightFootBoneName = "b_RightToeBase";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|Footsteps")
    FName LeftFootBoneName = "b_LeftToeBase";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|Footsteps")
    FFootstepFXData DefaultFootstepFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|Footsteps")
    TMap<ELocomotionType, FFootstepFXDataMap> FootstepsFXDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* DeathSound;

    virtual void BeginPlay() override;

private:
    bool MakeFootstepTrace(const FVector& TraceStart, const FVector& TraceEnd, FHitResult& HitResult);
};
