// Shooter_Game, All rights reserved.

#include "AI/Services/ShooterFindNearestActorService.h"
#include "AI/SHGAIController.h"
#include "Components/SHGAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UShooterFindNearestActorService::UShooterFindNearestActorService()
{
    NodeName = "Nearest Actor";
}

void UShooterFindNearestActorService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto ShooterAIController = Cast<ASHGAIController>(OwnerComp.GetAIOwner());
    if (!Blackboard || !ShooterAIController)
        return;

    const auto PerceptionComponent = ShooterAIController->FindComponentByClass<USHGAIPerceptionComponent>();
    if (!PerceptionComponent)
        return;

    const auto NearestActor = PerceptionComponent->GetNearestActor();
    if (NearestActor)
    {
        Blackboard->ClearValue(LostEnemyBlackboardKey.SelectedKeyName);
        Blackboard->SetValueAsObject(EnemyBlackboardKey.SelectedKeyName, NearestActor);

        return;
    }

    Blackboard->ClearValue(EnemyBlackboardKey.SelectedKeyName);
}
