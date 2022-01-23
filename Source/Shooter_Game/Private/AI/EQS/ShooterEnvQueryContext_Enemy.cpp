// Shooter_Game, All rights reserved.

#include "AI/EQS/ShooterEnvQueryContext_Enemy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"

void UShooterEnvQueryContext_Enemy::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
    const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!Blackboard)
        return;

    const auto EnemyActor = Blackboard->GetValueAsObject(EnemyBlackboardKeyName);

    UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(EnemyActor));
}
