// Shooter_Game, All rights reserved.


#include "Pickups/ShooterPickupHealthActor.h"
#include "Components/ShooterHealthComponent.h"
#include "ShooterUtils.h"

bool AShooterPickupHealthActor::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    return HealthComponent->TryToHeal(HealAmount);
}
