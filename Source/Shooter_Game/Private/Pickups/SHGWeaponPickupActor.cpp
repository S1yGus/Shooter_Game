// Shooter_Game, All rights reserved.

#include "Pickups/SHGWeaponPickupActor.h"
#include "Components/SHGHealthComponent.h"
#include "Components/SHGBaseWeaponComponent.h"
#include "Weapons/SHGBaseWeaponActor.h"

void ASHGWeaponPickupActor::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponClass);
}

bool ASHGWeaponPickupActor::TryToGivePickupTo(APawn* Pawn)
{
    const auto HealthComponent = Pawn->FindComponentByClass<USHGHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto WeaponComponent = Pawn->FindComponentByClass<USHGBaseWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddWeapon(WeaponClass);
}
