// Shooter_Game, All rights reserved.

#include "UI/HUD/SHGCrosshairUserWidget.h"
#include "Components/SHGBaseWeaponComponent.h"
#include "Components/Image.h"
#include "Brushes/SlateImageBrush.h"

void USHGCrosshairUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(CrosshairImage);
}

void USHGCrosshairUserWidget::OnNewPawn(APawn* NewPawn)
{
    Super::OnNewPawn(NewPawn);

    if (!NewPawn)
        return;

    if (const auto WeaponComponent = NewPawn->FindComponentByClass<USHGBaseWeaponComponent>())
    {
        WeaponComponent->OnWeaponChanged.AddUObject(this, &ThisClass::OnWeaponChanged);

        if (FWeaponUIData CurrentWeaponUIData; WeaponComponent->GetCurrentWeaponUIData(CurrentWeaponUIData))
        {
            OnWeaponChanged(CurrentWeaponUIData, FAmmoData{});
        }
    }
}

void USHGCrosshairUserWidget::OnWeaponChanged(const FWeaponUIData& CurrentWeaponUIData, const FAmmoData& CurrentAmmoData)
{
    UpdateCrosshairImage(CurrentWeaponUIData);
}

void USHGCrosshairUserWidget::UpdateCrosshairImage(const FWeaponUIData& CurrentWeaponUIData)
{
    CrosshairImage->SetBrush(FSlateImageBrush{CurrentWeaponUIData.CrossHairIcon, GetImageSize(CurrentWeaponUIData.CrossHairIcon), CrosshairTint});
}
