// Shooter_Game, All rights reserved.

#include "AI/Tasks/ShooterAttackLocationTaskNode.h"
#include "AIController.h"
#include "Components/ShooterAIWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

EBTNodeResult::Type UShooterAttackLocationTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Blackboard = OwnerComp.GetBlackboardComponent();
    const auto AIController = OwnerComp.GetAIOwner();
    if (!Blackboard || !AIController)
        return EBTNodeResult::Failed;

    const auto AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return EBTNodeResult::Failed;

    const auto AIWeaponComponent = AIPawn->FindComponentByClass<UShooterAIWeaponComponent>();
    if (!AIWeaponComponent)
        return EBTNodeResult::Failed;

    FEnvQueryRequest AttackLocationQueryRequest = FEnvQueryRequest(EQS, AIPawn);

    AttackLocationQueryRequest.SetFloatParam("Donut.InnerRadius", AIWeaponComponent->GetCurrentMinAttackDistance());
    AttackLocationQueryRequest.SetFloatParam("Donut.OuterRadius", AIWeaponComponent->GetCurrentMaxAttackDistance());

    AttackLocationQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &UShooterAttackLocationTaskNode::HandleQueryResult);
    return EBTNodeResult::Succeeded;
}

void UShooterAttackLocationTaskNode::HandleQueryResult(TSharedPtr<FEnvQueryResult> Result)
{
    if (Result.IsValid() && Result->IsSuccsessful())
    {
        Blackboard->SetValueAsVector(NewLocationBlackboardKey.SelectedKeyName, Result->GetItemAsLocation(0));
    }
}

UShooterAttackLocationTaskNode::UShooterAttackLocationTaskNode()
{
    NodeName = "Attack Location";
}
