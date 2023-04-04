// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SHGWeaponCoreTypes.h"
#include "SHGNeedWeaponBTDecorator.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGNeedWeaponBTDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    USHGNeedWeaponBTDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    EWeaponType WeaponType;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
