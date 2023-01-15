// Shooter_Game, All rights reserved.

#include "Components/SHGAIWeaponComponent.h"
#include "Weapons/SHGBaseWeaponActor.h"
#include "SHGWeaponCoreTypes.h"

void USHGAIWeaponComponent::StartFire()
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

void USHGAIWeaponComponent::NextWeapon()
{
    if (!CanEquip() || WeaponsMap.IsEmpty())
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

    if (NextWeaponIndex != CurrentWeaponIndex)
    {
        CurrentWeaponIndex = NextWeaponIndex;
        EquipWeapon(static_cast<EWeaponType>(CurrentWeaponIndex));
    }
}

bool USHGAIWeaponComponent::CheckWeapon(EWeaponType WeaponType) const
{
    return WeaponsMap.Contains(WeaponType);
}

bool USHGAIWeaponComponent::CheckWeaponAmmo(EWeaponType WeaponType) const
{
    if (!WeaponsMap.Contains(WeaponType))
        return false;

    return !WeaponsMap[WeaponType]->IsAmmoEmpty();
}

bool USHGAIWeaponComponent::CheckCurrentWeapon(EWeaponType WeaponType) const
{
    return CurrentWeapon->GetWeaponData().WeaponType == WeaponType;
}

float USHGAIWeaponComponent::GetCurrentMinAttackDistance() const
{
    if (!CurrentWeapon)
        return 0.0f;

    return CurrentWeapon->GetOptimalAttackDistance();
}

float USHGAIWeaponComponent::GetCurrentMaxAttackDistance() const
{
    if (!CurrentWeapon)
        return MAX_FLT;

    return CurrentWeapon->GetMaxAttackDistance();
}