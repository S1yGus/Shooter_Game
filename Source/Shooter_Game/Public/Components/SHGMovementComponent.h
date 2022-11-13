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

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = "1.0", ClampMax = "100.0"))
    float SprintSpeedMultiplier = 1.75f;
};
