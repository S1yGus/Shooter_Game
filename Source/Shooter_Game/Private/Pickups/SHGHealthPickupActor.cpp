// Shooter_Game, All rights reserved.

#include "Pickups/SHGHealthPickupActor.h"
#include "Components/SHGHealthComponent.h"

bool ASHGHealthPickupActor::TryToGivePickupTo(AActor* Actor)
{
    const auto HealthComponent = Actor->FindComponentByClass<USHGHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    return HealthComponent->TryToHeal(HealAmount);
}
