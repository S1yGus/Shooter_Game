// Shooter_Game, All rights reserved.

#include "Pickups/SHGHealthPickupActor.h"
#include "Components/SHGHealthComponent.h"

bool ASHGHealthPickupActor::TryToGivePickupTo(APawn* Pawn)
{
    const auto HealthComponent = Pawn->FindComponentByClass<USHGHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    return HealthComponent->TryToHeal(HealAmount);
}
