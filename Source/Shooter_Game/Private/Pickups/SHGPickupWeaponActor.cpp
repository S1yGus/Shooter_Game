// Shooter_Game, All rights reserved.

#include "Pickups/SHGPickupWeaponActor.h"
#include "Components/SHGHealthComponent.h"
#include "Components/SHGBaseWeaponComponent.h"

void ASHGPickupWeaponActor::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponClass);
}

bool ASHGPickupWeaponActor::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = PlayerPawn->FindComponentByClass<USHGHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto WeaponComponent = PlayerPawn->FindComponentByClass<USHGBaseWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddWeapon(WeaponClass);
}
