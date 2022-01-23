// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ShooterCoreTypes.h"
#include "ShooterBTDecoratorNeedWeapon.generated.h"

class AShooterBaseWeaponActor;

UCLASS()
class SHOOTER_GAME_API UShooterBTDecoratorNeedWeapon : public UBTDecorator
{
    GENERATED_BODY()

public:
    UShooterBTDecoratorNeedWeapon();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyBlackboardKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float RandomWeightWithEnemy = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float RandomWeightNoEnemy = 0.9f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
