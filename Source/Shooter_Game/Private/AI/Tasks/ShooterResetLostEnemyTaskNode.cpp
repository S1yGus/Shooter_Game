// Shooter_Game, All rights reserved.

#include "AI/Tasks/ShooterResetLostEnemyTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UShooterResetLostEnemyTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
        return EBTNodeResult::Failed;

    Blackboard->SetValueAsObject(LostEnemyBlackboardKey.SelectedKeyName, nullptr);

    return EBTNodeResult::Succeeded;
}
