// Shooter_Game, All rights reserved.

#include "AI/Decorators/SHGNeedHealingBTDecorator.h"
#include "AIController.h"
#include "Components/SHGHealthComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USHGNeedHealingBTDecorator::USHGNeedHealingBTDecorator()
{
    NodeName = "Need Healing";
}

bool USHGNeedHealingBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    const auto AIController = OwnerComp.GetAIOwner();
    if (!BlackboardComponent || !AIController)
        return false;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return false;

    const auto HealthComponent = AIPawn->FindComponentByClass<USHGHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const bool bHasEnemy = static_cast<bool>(BlackboardComponent->GetValueAsObject(EnemyBlackboardKey.SelectedKeyName));

    return HealthComponent->GetHealthPercent() < (bHasEnemy ? HealthThresholdWithEnemy : HealthThresholdWithoutEnemy);
}