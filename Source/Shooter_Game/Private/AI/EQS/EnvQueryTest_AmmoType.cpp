// Shooter_Game, All rights reserved.

#include "AI/EQS/EnvQueryTest_AmmoType.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "Pickups/ShooterPickupAmmoActor.h"
#include "Weapons/SHGBaseWeaponActor.h"

UEnvQueryTest_AmmoType::UEnvQueryTest_AmmoType(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_AmmoType::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* QueryOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(QueryOwner, QueryInstance.QueryID);

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto Pickup = Cast<AShooterPickupAmmoActor>(ItemActor);
        if (!Pickup)
            continue;

        It.SetScore(TestPurpose, FilterType, NeededAmmoClass == Pickup->GetWeaponClass(), BoolValue.GetValue());
    }
}
