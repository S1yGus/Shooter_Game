// Shooter_Game, All rights reserved.

#include "Pickups/ShooterPickupHealthActor.h"
#include "Components/SHGHealthComponent.h"
#include "SHGUtils.h"

bool AShooterPickupHealthActor::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = SHGUtils::GetSHGComponent<USHGHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    return HealthComponent->TryToHeal(HealAmount);
}
