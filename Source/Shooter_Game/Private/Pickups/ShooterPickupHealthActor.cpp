// Shooter_Game, All rights reserved.

#include "Pickups/ShooterPickupHealthActor.h"
#include "Components/SHGHealthComponent.h"
#include "ShooterUtils.h"

bool AShooterPickupHealthActor::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<USHGHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    return HealthComponent->TryToHeal(HealAmount);
}
