// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SHGMovementComponent.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    virtual float GetMaxSpeed() const override;

    void InForceField(bool bCondition);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = "1.0"))
    float SprintSpeedMultiplier = 1.75f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = "0.0"))
    float GroundFrictionInForceField = 0.0f;

protected:
    float DefaultGroundFriction = 0.0f;
};
