// Shooter_Game, All rights reserved.

#include "AI/Services/SHGAttackService.h"
#include "Components/SHGBaseWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

USHGAttackService::USHGAttackService()
{
    NodeName = "Attack";
}

void USHGAttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    const auto AIController = OwnerComp.GetAIOwner();
    if (!BlackboardComponent && !AIController)
        return;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return;

    if (const auto WeaponComponent = AIPawn->FindComponentByClass<USHGBaseWeaponComponent>())
    {
        BlackboardComponent->GetValueAsObject(EnemyBlackboardKey.SelectedKeyName) ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
    }
}
