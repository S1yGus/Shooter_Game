// Shooter_Game, All rights reserved.


#include "Pickups/ShooterPickupWeaponActor.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterUtils.h"

bool AShooterPickupWeaponActor::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(PlayerPawn);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->TryToAddWeapon(WeaponData);
}