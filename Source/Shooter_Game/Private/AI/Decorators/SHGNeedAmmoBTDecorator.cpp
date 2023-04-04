// Shooter_Game, All rights reserved.

#include "AI/Decorators/SHGNeedAmmoBTDecorator.h"
#include "Components/SHGAIWeaponComponent.h"
#include "Components/SHGHealthComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USHGNeedAmmoBTDecorator::USHGNeedAmmoBTDecorator()
{
    NodeName = "Need Ammo";
}

bool USHGNeedAmmoBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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
    const auto AIWeaponComponent = AIPawn->FindComponentByClass<USHGAIWeaponComponent>();
    if (!AIWeaponComponent || !AIWeaponComponent->NeedAmmo(WeaponType, bHasEnemy))
        return false;

    return true;
}
