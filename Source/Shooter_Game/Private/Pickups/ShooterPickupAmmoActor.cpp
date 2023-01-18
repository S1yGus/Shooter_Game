// Shooter_Game, All rights reserved.

#include "Pickups/ShooterPickupAmmoActor.h"
#include "Components/SHGHealthComponent.h"
#include "Components/SHGBaseWeaponComponent.h"
#include "SHGUtils.h"

bool AShooterPickupAmmoActor::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = SHGUtils::GetSHGComponent<USHGHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto WeaponComponent = SHGUtils::GetSHGComponent<USHGBaseWeaponComponent>(PlayerPawn);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddAmmo(AmmoWeaponClass, ClipsAmount);
}
