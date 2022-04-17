// Shooter_Game, All rights reserved.

#include "UI/HUD/ShooterCrosshairUserWidget.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/Image.h"
#include "Brushes/SlateImageBrush.h"

void UShooterCrosshairUserWidget::OnNewPawn(APawn* NewPawn)
{
    Super::OnNewPawn(NewPawn);

    if (!NewPawn)
        return;

    if (const auto WeaponComponent = NewPawn->FindComponentByClass<UShooterWeaponComponent>())
    {
        WeaponComponent->OnWeaponChanged.AddUObject(this, &UShooterCrosshairUserWidget::OnWeaponChanged);
    }

    FWeaponUIData CurrentWeaponUIData;
    if (GetCurrentWeaponUIData(CurrentWeaponUIData))
    {
        OnWeaponChanged(CurrentWeaponUIData, FAmmoData());
    }
}

void UShooterCrosshairUserWidget::OnWeaponChanged(const FWeaponUIData& CurrentWeaponUIData, const FAmmoData& CurrentAmmoData)
{
    UpdateCrosshairImage(CurrentWeaponUIData);
}

void UShooterCrosshairUserWidget::UpdateCrosshairImage(const FWeaponUIData& CurrentWeaponUIData)
{
    if (!CrosshairImage)
        return;

    FSlateImageBrush Brush(CurrentWeaponUIData.CrossHairIcon, GetImageSize(CurrentWeaponUIData.CrossHairIcon), CrosshairTint);
    CrosshairImage->SetBrush(Brush);
}