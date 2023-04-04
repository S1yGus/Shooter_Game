// Shooter_Game, All rights reserved.

#include "AI/Decorators/SHGNeedWeaponBTDecorator.h"
#include "Components/SHGAIWeaponComponent.h"
#include "Components/SHGHealthComponent.h"
#include "AIController.h"

USHGNeedWeaponBTDecorator::USHGNeedWeaponBTDecorator()
{
    NodeName = "Need Weapon";
}

bool USHGNeedWeaponBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

    const auto AIWeaponComponent = AIPawn->FindComponentByClass<USHGAIWeaponComponent>();
    if (!AIWeaponComponent)
        return false;

    return !AIWeaponComponent->CheckForWeapon(WeaponType);
}
