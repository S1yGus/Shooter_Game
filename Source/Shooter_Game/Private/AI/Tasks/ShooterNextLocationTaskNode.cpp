// Shooter_Game, All rights reserved.

#include "AI/Tasks/ShooterNextLocationTaskNode.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UShooterNextLocationTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard)
        return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
        return EBTNodeResult::Failed;

    const auto NavigationSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavigationSystem)
        return EBTNodeResult::Failed;

    FVector CenterLocation = Pawn->GetActorLocation();
    if (!SelfCenter)
    {
        const auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorBlackboardKey.SelectedKeyName));
        CenterLocation = CenterActor->GetActorLocation();
    }

    FNavLocation NavLocation;
    if (!NavigationSystem->GetRandomReachablePointInRadius(CenterLocation, PatrolRadius, NavLocation))
        return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(NewLocationBlackboardKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;
}

UShooterNextLocationTaskNode::UShooterNextLocationTaskNode()
{
    NodeName = "Next Location";
}
