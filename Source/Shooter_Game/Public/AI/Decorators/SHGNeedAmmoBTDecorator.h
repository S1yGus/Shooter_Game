// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SHGWeaponCoreTypes.h"
#include "SHGNeedAmmoBTDecorator.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGNeedAmmoBTDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    USHGNeedAmmoBTDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyBlackboardKey;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
