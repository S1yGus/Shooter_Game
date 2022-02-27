// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "ShooterLocomotionCameraShake.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterLocomotionCameraShake : public UMatineeCameraShake
{
    GENERATED_BODY()

protected:
    UFUNCTION(BlueprintCallable)
    float GetPlayerVelocityNormalized() const;

    UFUNCTION(BlueprintCallable)
    bool IsZooming() const;
};
