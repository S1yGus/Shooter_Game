// Shooter_Game, All rights reserved.

#include "AI/Decorators/ShooterBTDecoratorNeedHealing.h"
#include "AIController.h"
#include "Components/ShooterHealthComponent.h"

UShooterBTDecoratorNeedHealing::UShooterBTDecoratorNeedHealing()
{
    NodeName = "Need Healing";
}

bool UShooterBTDecoratorNeedHealing::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController)
        return false;

    const auto HealthComponent = AIController->GetPawn()->FindComponentByClass<UShooterHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    return HealthComponent->GetHealthPercent() < HealthPercent;
}