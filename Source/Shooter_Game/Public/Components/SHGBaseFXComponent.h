// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHGFXCoreTypes.h"
#include "SHGBaseFXComponent.generated.h"

class USoundCue;
class ASHGImpactIndicatorActor;
class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGBaseFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USHGBaseFXComponent();

    void MakeFootstepsFX(const FFootstepNotifyData& FootstepNotifyData);
    void SpawnImpactIndicator(float DamageAmount, const FVector& HitLocation, UPhysicalMaterial* PhysicalMaterial);
    void PlayDeathSound();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    bool bShowImpactIndicators = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX", Meta = (EditCondition = "ShowImpactIndicators"))
    TSubclassOf<ASHGImpactIndicatorActor> ImpactIndicatorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|Footsteps")
    FName RightFootBoneName = "b_RightToeBase";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|Footsteps")
    FName LeftFootBoneName = "b_LeftToeBase";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|Footsteps")
    FFootstepsFXData DefaultFootstepsFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX|Footsteps")
    TMap<UPhysicalMaterial*, FFootstepsFXData> FootstepsFXDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* DeathSound;

    virtual void BeginPlay() override;

private:
    bool MakeFootstepTrace(const FVector& TraceStart, const FVector& TraceEnd, FHitResult& HitResult, const ACharacter* OwnerCharacter);
};
