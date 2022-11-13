// Shooter_Game, All rights reserved.

#include "Pickups/ShooterPickupWeaponActor.h"
#include "Components/SHGHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterUtils.h"

bool AShooterPickupWeaponActor::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<USHGHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<USHGBaseWeaponComponent>(PlayerPawn);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddWeapon(WeaponData);
}