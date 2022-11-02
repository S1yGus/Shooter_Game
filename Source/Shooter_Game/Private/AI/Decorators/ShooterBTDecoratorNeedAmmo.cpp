// Shooter_Game, All rights reserved.

#include "AI/Decorators/ShooterBTDecoratorNeedAmmo.h"
#include "Components/ShooterAIWeaponComponent.h"
#include "Components/SHGHealthComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UShooterBTDecoratorNeedAmmo::UShooterBTDecoratorNeedAmmo()
{
    NodeName = "Need Ammo";
}

bool UShooterBTDecoratorNeedAmmo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
        return false;

    float RandomWeight = Blackboard->GetValueAsObject(EnemyBlackboardKey.SelectedKeyName) ? RandomWeightWithEnemy : RandomWeightNoEnemy;
    if (!UKismetMathLibrary::RandomBoolWithWeight(RandomWeight))
        return false;

    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController)
        return false;

    const auto HealthComponent = AIController->GetPawn()->FindComponentByClass<USHGHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto AIWeaponComponent = AIController->GetPawn()->FindComponentByClass<UShooterAIWeaponComponent>();
    if (!AIWeaponComponent || !AIWeaponComponent->CheckWeapon(WeaponType))
        return false;

    return !AIWeaponComponent->CheckWeaponAmmo(WeaponType);
}