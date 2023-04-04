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
    BoolValue.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        if (const auto Pickup = Cast<ASHGBasePickupActor>(GetItemActor(QueryInstance, It.GetIndex())))
        {
            It.SetScore(TestPurpose, FilterType, Pickup->CanBeTaken(), BoolValue.GetValue());
        }
    }
}
