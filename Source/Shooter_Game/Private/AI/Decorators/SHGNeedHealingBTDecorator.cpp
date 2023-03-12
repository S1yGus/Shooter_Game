// Shooter_Game, All rights reserved.

#include "AI/Decorators/SHGNeedHealingBTDecorator.h"
#include "AIController.h"
#include "Components/SHGHealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

USHGNeedHealingBTDecorator::USHGNeedHealingBTDecorator()
{
    NodeName = "Need Healing";
}

bool USHGNeedHealingBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController)
        return false;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return false;

    const auto HealthComponent = AIPawn->FindComponentByClass<USHGHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    return (HealthComponent->GetHealthPercent() < HealthPercentThreshold) && UKismetMathLibrary::RandomBoolWithWeight(Probability);
}
