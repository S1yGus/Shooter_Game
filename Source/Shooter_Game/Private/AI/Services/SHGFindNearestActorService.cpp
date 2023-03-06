// Shooter_Game, All rights reserved.

#include "AI/Services/SHGFindNearestActorService.h"
#include "AIController.h"
#include "Components/SHGAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USHGFindNearestActorService::USHGFindNearestActorService()
{
    NodeName = "Find Nearest Actor";
}

void USHGFindNearestActorService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    const auto AIController = OwnerComp.GetAIOwner();
    if (!BlackboardComponent || !AIController)
        return;

    if (const auto PerceptionComponent = AIController->FindComponentByClass<USHGAIPerceptionComponent>())
    {
        BlackboardComponent->SetValueAsObject(EnemyBlackboardKey.SelectedKeyName, PerceptionComponent->GetNearestActor());
    }
}
