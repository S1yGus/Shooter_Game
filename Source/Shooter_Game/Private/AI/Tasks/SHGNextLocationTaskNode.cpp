// Shooter_Game, All rights reserved.

#include "AI/Tasks/SHGNextLocationTaskNode.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

USHGNextLocationTaskNode::USHGNextLocationTaskNode()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USHGNextLocationTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto AIController = OwnerComp.GetAIOwner();
    const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!AIController || !BlackboardComponent)
        return EBTNodeResult::Failed;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return EBTNodeResult::Failed;

    const auto NavigationSystem = UNavigationSystemV1::GetCurrent(AIPawn);
    if (!NavigationSystem)
        return EBTNodeResult::Failed;

    FVector CenterLocation = AIPawn->GetActorLocation();
    if (!bSelfCenter)
    {
        const auto CenterActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorBlackboardKey.SelectedKeyName));
        CenterLocation = CenterActor->GetActorLocation();
    }

    FNavLocation NavLocation;
    if (!NavigationSystem->GetRandomReachablePointInRadius(CenterLocation, Radius, NavLocation))
        return EBTNodeResult::Failed;

    BlackboardComponent->SetValueAsVector(NewLocationBlackboardKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;
}
