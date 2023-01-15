// Shooter_Game, All rights reserved.

#include "AI/EQS/EnvQueryTest_WeaponType.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "Pickups/SHGPickupWeaponActor.h"
#include "Weapons/SHGBaseWeaponActor.h"

UEnvQueryTest_WeaponType::UEnvQueryTest_WeaponType(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_WeaponType::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* QueryOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(QueryOwner, QueryInstance.QueryID);

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto Pickup = Cast<ASHGPickupWeaponActor>(ItemActor);
        if (!Pickup)
            continue;

        It.SetScore(TestPurpose, FilterType, NeededWeaponClass == Pickup->GetWeaponClass(), BoolValue.GetValue());
    }
}