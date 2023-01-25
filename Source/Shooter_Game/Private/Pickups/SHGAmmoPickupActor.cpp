// Shooter_Game, All rights reserved.

#include "Pickups/SHGAmmoPickupActor.h"
#include "Components/SHGHealthComponent.h"
#include "Components/SHGBaseWeaponComponent.h"

bool ASHGAmmoPickupActor::TryToGivePickupTo(AActor* Actor)
{
    const auto HealthComponent = Actor->FindComponentByClass<USHGHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto WeaponComponent = Actor->FindComponentByClass<USHGBaseWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddAmmo(AmmoWeaponClass, ClipsAmount);
}
