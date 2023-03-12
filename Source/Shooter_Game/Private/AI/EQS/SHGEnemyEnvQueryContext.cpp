// Shooter_Game, All rights reserved.

#include "AI/EQS/SHGEnemyEnvQueryContext.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"

void USHGEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
    const auto BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!BlackboardComponent)
        return;

    const auto EnemyActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyBlackboardKeyName));
    UEnvQueryItemType_Actor::SetContextHelper(ContextData, EnemyActor);
}
