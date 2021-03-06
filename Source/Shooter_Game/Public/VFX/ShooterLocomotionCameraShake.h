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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    float ZommedCameraShakeScale = 0.0f;

    UFUNCTION(BlueprintCallable)
    float GetPlayerVelocityNormalized() const;

    UFUNCTION(BlueprintCallable)
    bool IsZooming() const;
};
