// Shooter_Game, All rights reserved.

#include "AI/Tasks/SHGNextAttackLocationTaskNode.h"
#include "AIController.h"
#include "Components/SHGAIWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

USHGNextAttackLocationTaskNode::USHGNextAttackLocationTaskNode()
{
    NodeName = "Finding Next Attack Location";
}

EBTNodeResult::Type USHGNextAttackLocationTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto AIController = OwnerComp.GetAIOwner();
    if (!AIController)
        return EBTNodeResult::Failed;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return EBTNodeResult::Failed;

    const auto AIWeaponComponent = AIPawn->FindComponentByClass<USHGAIWeaponComponent>();
    if (!AIWeaponComponent)
        return EBTNodeResult::Failed;

    FEnvQueryRequest NextAttackLocationQueryRequest{EQS, AIController};
    NextAttackLocationQueryRequest.SetFloatParam(InnerRadiusParamName, AIWeaponComponent->GetCurrentMinAttackDistance());
    NextAttackLocationQueryRequest.SetFloatParam(OuterRadiusParamName, AIWeaponComponent->GetCurrentMaxAttackDistance());
    NextAttackLocationQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ThisClass::HandleQueryResult);
    return EBTNodeResult::Succeeded;
}

void USHGNextAttackLocationTaskNode::HandleQueryResult(TSharedPtr<FEnvQueryResult> Result)
{
    if (Result.IsValid() && Result->IsSuccsessful() && !Result->Items.IsEmpty())
    {
        if (const auto AIController = Cast<AAIController>(Result->Owner.Get()))
        {
            if (const auto BlackboardComponent = AIController->GetBlackboardComponent())
            {
                BlackboardComponent->SetValueAsVector(NewLocationBlackboardKey.SelectedKeyName, Result->GetItemAsLocation(0));
            }
        }
    }
}
