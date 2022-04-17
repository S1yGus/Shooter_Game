// Shooter_Game, All rights reserved.

#include "UI/HUD/ShooterWeaponInfoUserWidget.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Brushes/SlateImageBrush.h"
#include "ShooterUtils.h"

void UShooterWeaponInfoUserWidget::OnNewPawn(APawn* NewPawn)
{
    Super::OnNewPawn(NewPawn);

    if (!NewPawn)
        return;

    if (const auto WeaponComponent = NewPawn->FindComponentByClass<UShooterWeaponComponent>())
    {
        WeaponComponent->OnWeaponChanged.AddUObject(this, &UShooterWeaponInfoUserWidget::OnWeaponChanged);
        WeaponComponent->OnAmmoInfoChanged.AddUObject(this, &UShooterWeaponInfoUserWidget::UpdateAmmoInfo);
    }

    FWeaponUIData CurrentWeaponUIData;
    FAmmoData CurrentAmmoData;
    if (GetCurrentWeaponUIData(CurrentWeaponUIData) && GetCurrentWeaponAmmoData(CurrentAmmoData))
    {
        OnWeaponChanged(CurrentWeaponUIData, CurrentAmmoData);
    }
}

void UShooterWeaponInfoUserWidget::OnWeaponChanged(const FWeaponUIData& CurrentWeaponUIData, const FAmmoData& CurrentAmmoData)
{
    UpdateWeaponIconImage(CurrentWeaponUIData);
    UpdateAmmoInfo(CurrentAmmoData);
}

bool UShooterWeaponInfoUserWidget::GetCurrentWeaponAmmoData(FAmmoData& Data) const
{
    if (!GetOwningPlayerPawn())
        return false;

    const auto WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetCurrentWeaponAmmoData(Data);
}

void UShooterWeaponInfoUserWidget::UpdateWeaponIconImage(const FWeaponUIData& CurrentWeaponUIData)
{
    if (!WeaponIconImage)
        return;

    FSlateImageBrush Brush(CurrentWeaponUIData.MainIcon, GetImageSize(CurrentWeaponUIData.MainIcon), WeaponIconTint);
    WeaponIconImage->SetBrush(Brush);
}

void UShooterWeaponInfoUserWidget::UpdateAmmoInfo(const FAmmoData& CurrentAmmoData)
{
    if (!BulletsTextBlock || !ClipsTextBlock)
        return;

    const auto NewBulletsStr = ShooterUtils::FormatStringFromInt(CurrentAmmoData.BulletsInClip, 2, '0');
    BulletsTextBlock->SetText(FText::FromString(NewBulletsStr));

    auto NewClipsStr = ShooterUtils::FormatStringFromInt(CurrentAmmoData.Clips, 2, '0');
    if (CurrentAmmoData.InfiniteClips)
    {
        NewClipsStr = InfiniteClipsSymbol;
    }
    NewClipsStr = FString::Printf(TEXT("/ %s"), *NewClipsStr);
    ClipsTextBlock->SetText(FText::FromString(NewClipsStr));
}