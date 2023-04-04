// Shooter_Game, All rights reserved.

#include "AI/EQS/EnvQueryTest_AmmoType.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "Pickups/SHGAmmoPickupActor.h"
#include "Weapons/SHGBaseWeaponActor.h"

UEnvQueryTest_AmmoType::UEnvQueryTest_AmmoType(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_AmmoType::RunTest(FEnvQueryInstance& QueryInstance) const
{
    BoolValue.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        if (const auto Pickup = Cast<ASHGAmmoPickupActor>(GetItemActor(QueryInstance, It.GetIndex())))
        {
            It.SetScore(TestPurpose, FilterType, Pickup->GetWeaponClass()->IsChildOf(NeededAmmoClass), BoolValue.GetValue());
        }
    }
}
