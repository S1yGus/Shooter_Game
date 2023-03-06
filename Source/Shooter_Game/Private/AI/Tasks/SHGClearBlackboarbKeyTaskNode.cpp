// Shooter_Game, All rights reserved.

#include "AI/Tasks/SHGClearBlackboarbKeyTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"

USHGClearBlackboarbKeyTaskNode::USHGClearBlackboarbKeyTaskNode()
{
    NodeName = "Clear Blackboarb Key";
}

EBTNodeResult::Type USHGClearBlackboarbKeyTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (const auto BlackboardComponent = OwnerComp.GetBlackboardComponent())
    {
        BlackboardComponent->ClearValue(BlackboardKey.SelectedKeyName);

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
