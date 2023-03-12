// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SHGNeedHealingBTDecorator.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGNeedHealingBTDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    USHGNeedHealingBTDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float HealthPercentThreshold = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Probability = 0.9f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
