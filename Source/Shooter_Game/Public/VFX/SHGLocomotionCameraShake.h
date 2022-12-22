// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "SHGLocomotionCameraShake.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGLocomotionCameraShake : public UCameraShakeBase
{
    GENERATED_BODY()

public:
    USHGLocomotionCameraShake(const FObjectInitializer& ObjInit);

    void UpdateLocomotionCameraShake(float InRotationAmplitudeMultiplier, float InRotationFrequencyMultiplier);
};
