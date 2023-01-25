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

bool ASHGWeaponPickupActor::TryToGivePickupTo(AActor* Actor)
{
    const auto HealthComponent = Actor->FindComponentByClass<USHGHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto WeaponComponent = Actor->FindComponentByClass<USHGBaseWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddWeapon(WeaponClass);
}
