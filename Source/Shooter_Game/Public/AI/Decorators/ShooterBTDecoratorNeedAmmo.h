// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ShooterCoreTypes.h"
#include "ShooterBTDecoratorNeedAmmo.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterBTDecoratorNeedAmmo : public UBTDecorator
{
    GENERATED_BODY()

public:
    UShooterBTDecoratorNeedAmmo();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyBlackboardKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float RandomWeightWithEnemy = 0.6f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float RandomWeightNoEnemy = 0.9f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
