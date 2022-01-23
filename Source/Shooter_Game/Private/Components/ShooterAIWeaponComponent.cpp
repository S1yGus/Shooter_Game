// Shooter_Game, All rights reserved.

#include "Components/ShooterAIWeaponComponent.h"
#include "Weapons/ShooterBaseWeaponActor.h"
#include "ShooterCoreTypes.h"

void UShooterAIWeaponComponent::StartFire()
{
    if (!CanFire())
        return;

    if (CurrentWeapon->IsAmmoEmpty())
    {
        NextWeapon();
    }
    else
    {
        CurrentWeapon->StartFire();
    }
}

void UShooterAIWeaponComponent::NextWeapon()
{
    if (!CanEquip() || WeaponsMap.Num() == 0)
        return;

    int32 NextWeaponIndex = (CurrentWeaponIndex + 1) % static_cast<int32>(EWeaponType::Max);

    while (NextWeaponIndex != CurrentWeaponIndex)
    {
        if (WeaponsMap.Contains(static_cast<EWeaponType>(NextWeaponIndex)))
        {
            if (!WeaponsMap[static_cast<EWeaponType>(NextWeaponIndex)]->IsAmmoEmpty())
                break;
        }

        NextWeaponIndex = (NextWeaponIndex + 1) % static_cast<int32>(EWeaponType::Max);
    }

    if (NextWeaponIndex == CurrentWeaponIndex)
        return;

    CurrentWeaponIndex = NextWeaponIndex;
    EquipWeapon(static_cast<EWeaponType>(CurrentWeaponIndex));
}

bool UShooterAIWeaponComponent::CheckWeapon(EWeaponType WeaponType) const
{
    return WeaponsMap.Contains(WeaponType);
}

bool UShooterAIWeaponComponent::CheckWeaponAmmo(EWeaponType WeaponType) const
{
    if (!WeaponsMap.Contains(WeaponType))
        return false;

    return !WeaponsMap[WeaponType]->IsAmmoEmpty();
}

bool UShooterAIWeaponComponent::CheckCurrentWeapon(EWeaponType WeaponType) const
{
    return CurrentWeapon->GetWeaponType() == WeaponType;
}

float UShooterAIWeaponComponent::GetCurrentMinAttackDistance() const
{
    if (!CurrentWeapon)
        return 0.0f;

    return CurrentWeapon->GetOptimalAttackDistance();
}

float UShooterAIWeaponComponent::GetCurrentMaxAttackDistance() const
{
    if (!CurrentWeapon)
        return MAX_FLT;

    return CurrentWeapon->GetMaxAttackDistance();
}