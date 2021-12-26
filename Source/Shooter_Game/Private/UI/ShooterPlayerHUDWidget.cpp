// Shooter_Game, All rights reserved.


#include "UI/ShooterPlayerHUDWidget.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterUtils.h"

float UShooterPlayerHUDWidget::GetHelthPercent() const
{
    const auto HealthComponent =
        ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent)
        return 0.0f;
    
    return HealthComponent->GetHealthPercent();
}

bool UShooterPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& Data) const
{
    const auto WeaponComponent =
        ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetCurrentWeaponUIData(Data);
}

bool UShooterPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& Data) const
{
    const auto WeaponComponent =
        ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetCurrentWeaponAmmoData(Data);
}

bool UShooterPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent =
        ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent)
        return false;

    return HealthComponent && !HealthComponent->IsDead();
}

bool UShooterPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}