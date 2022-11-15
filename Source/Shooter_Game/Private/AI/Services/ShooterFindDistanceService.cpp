// Shooter_Game, All rights reserved.

#include "AI/Services/ShooterFindDistanceService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UShooterFindDistanceService::UShooterFindDistanceService()
{
    NodeName = "Find Distance";
}

void UShooterFindDistanceService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto AIController = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!AIController || !Blackboard)
        return;

    const auto Enemy = Cast<AActor>(Blackboard->GetValueAsObject(EnemyBlackboardKey.SelectedKeyName));
    if (!Enemy)
        return;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return;

    float Distance = (Enemy->GetActorLocation() - AIPawn->GetActorLocation()).Size();
    Blackboard->SetValueAsFloat(DistanceBetweenBlackboardKey.SelectedKeyName, Distance);
}
