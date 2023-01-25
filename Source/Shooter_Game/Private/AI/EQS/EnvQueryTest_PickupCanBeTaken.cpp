// Shooter_Game, All rights reserved.

#include "AI/EQS/EnvQueryTest_PickupCanBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "Pickups/SHGBasePickupActor.h"

UEnvQueryTest_PickupCanBeTaken::UEnvQueryTest_PickupCanBeTaken(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCanBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* QueryOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(QueryOwner, QueryInstance.QueryID);

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto Pickup = Cast<ASHGBasePickupActor>(ItemActor);
        if (!Pickup)
            continue;

        It.SetScore(TestPurpose, FilterType, Pickup->CanBeTaken(), BoolValue.GetValue());
    }
}