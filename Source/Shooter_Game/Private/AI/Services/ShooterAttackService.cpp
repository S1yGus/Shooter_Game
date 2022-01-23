// Shooter_Game, All rights reserved.

#include "AI/Services/ShooterAttackService.h"
#include "Components/ShooterWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UShooterAttackService::UShooterAttackService()
{
    NodeName = "Attack";
}

void UShooterAttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto AIController = OwnerComp.GetAIOwner();
    if (!Blackboard && !AIController)
        return;

    const auto WeaponComponent = AIController->GetPawn()->FindComponentByClass<UShooterWeaponComponent>();

    if (WeaponComponent)
    {
        Blackboard->GetValueAsObject(EnemyBlackboardKey.SelectedKeyName) ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
    }
}
