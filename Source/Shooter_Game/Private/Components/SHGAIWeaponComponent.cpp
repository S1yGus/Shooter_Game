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

bool USHGAIWeaponComponent::NeedAmmo(EWeaponType WeaponType, bool bHasEnemy) const
{
    if (!WeaponsMap.Contains(WeaponType))
        return false;

    return bHasEnemy ? WeaponsMap[WeaponType]->IsAmmoEmpty() : !WeaponsMap[WeaponType]->IsNumberOfClipsMax();
}

bool USHGAIWeaponComponent::IsNoAmmoAtAll() const
{
    bool bNoAmmoAtAll = true;
    for (const auto& [Type, Weapon] : WeaponsMap)
    {
        if (Weapon && !Weapon->IsAmmoEmpty())
        {
            bNoAmmoAtAll = false;
            break;
        }
    }

    return bNoAmmoAtAll;
}

float USHGAIWeaponComponent::GetCurrentMinAttackDistance() const
{
    if (!CurrentWeapon)
        return 0.0f;

    return CurrentWeapon->GetMinAttackDistance();
}

float USHGAIWeaponComponent::GetCurrentMaxAttackDistance() const
{
    if (!CurrentWeapon)
        return MAX_FLT;

    return CurrentWeapon->GetMaxAttackDistance();
}
