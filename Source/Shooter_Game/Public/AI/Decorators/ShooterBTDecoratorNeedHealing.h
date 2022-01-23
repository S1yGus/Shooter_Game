// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ShooterBTDecoratorNeedHealing.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterBTDecoratorNeedHealing : public UBTDecorator
{
    GENERATED_BODY()

public:
    UShooterBTDecoratorNeedHealing();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = 0.5f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
