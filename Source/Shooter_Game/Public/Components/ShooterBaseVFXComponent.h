// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterBaseVFXComponent.generated.h"

class USoundCue;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API UShooterBaseVFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UShooterBaseVFXComponent();

    void MakeFootstepVFX(const FFootstepNotifyData& FootstepNotifyData);
    void PlayDeathSound();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX|Footsteps")
    FName RightFootBoneName = "b_RightToeBase";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX|Footsteps")
    FName LeftFootBoneName = "b_LeftToeBase";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX|Footsteps")
    FFootstepFXData DefaultFootstepFXData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX|Footsteps")
    TMap<ELocomotionType, FFootstepFXDataMap> FootstepsFXDataMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* DeathSound;

    virtual void BeginPlay() override;

private:
    bool MakeFootstepTrace(const FVector& TraceStart, const FVector& TraceEnd, FHitResult& HitResult);
};
