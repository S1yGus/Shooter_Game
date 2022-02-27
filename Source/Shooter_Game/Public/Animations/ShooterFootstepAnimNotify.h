// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ShooterCoreTypes.h"
#include "ShooterFootstepAnimNotify.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterFootstepAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    FFootstepNotifyData FootstepNotifyData;
};
