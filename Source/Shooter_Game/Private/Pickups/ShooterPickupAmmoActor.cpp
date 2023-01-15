// Shooter_Game, All rights reserved.

#include "Pickups/ShooterPickupAmmoActor.h"
#include "Components/SHGHealthComponent.h"
#include "Components/SHGBaseWeaponComponent.h"
#include "ShooterUtils.h"

bool AShooterPickupAmmoActor::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<USHGHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<USHGBaseWeaponComponent>(PlayerPawn);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddAmmo(AmmoWeaponClass, ClipsAmount);
}
