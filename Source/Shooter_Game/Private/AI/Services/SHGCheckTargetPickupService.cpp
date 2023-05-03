// Shooter_Game, All rights reserved.

#include "AI/Services/SHGCheckTargetPickupService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pickups/SHGBasePickupActor.h"

USHGCheckTargetPickupService::USHGCheckTargetPickupService()
{
    NodeName = "Check Target Pickup";
}

void USHGCheckTargetPickupService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent)
        return;

    const auto TargetPickup = Cast<ASHGBasePickupActor>(BlackboardComponent->GetValueAsObject(TargetPickupBlackboardKey.SelectedKeyName));
    if (!IsValid(TargetPickup) || !TargetPickup->CanBeTaken())
    {
        BlackboardComponent->ClearValue(TargetPickupBlackboardKey.SelectedKeyName);
    }
}
