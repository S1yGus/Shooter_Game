// Shooter_Game, All rights reserved.

#include "UI/HUD/SHGWeaponInfoUserWidget.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Brushes/SlateImageBrush.h"
#include "ShooterUtils.h"

constexpr static int32 AmmoInfoMinStrLen = 2;
constexpr static char AmmoInfoFillChar = '0';

void USHGWeaponInfoUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(WeaponIconImage);
    check(BulletsTextBlock);
    check(ClipsTextBlock);
}

void USHGWeaponInfoUserWidget::OnNewPawn(APawn* NewPawn)
{
    Super::OnNewPawn(NewPawn);

    if (!NewPawn)
        return;

    if (const auto WeaponComponent = NewPawn->FindComponentByClass<USHGBaseWeaponComponent>())
    {
        WeaponComponent->OnWeaponChanged.AddUObject(this, &ThisClass::OnWeaponChanged);
        WeaponComponent->OnAmmoInfoChanged.AddUObject(this, &ThisClass::UpdateAmmoInfo);

        FWeaponUIData CurrentWeaponUIData;
        FAmmoData CurrentAmmoData;
        if (WeaponComponent->GetCurrentWeaponUIData(CurrentWeaponUIData) && WeaponComponent->GetCurrentWeaponAmmoData(CurrentAmmoData))
        {
            OnWeaponChanged(CurrentWeaponUIData, CurrentAmmoData);
        }
    }
}

void USHGWeaponInfoUserWidget::OnWeaponChanged(const FWeaponUIData& CurrentWeaponUIData, const FAmmoData& CurrentAmmoData)
{
    UpdateWeaponIconImage(CurrentWeaponUIData);
    UpdateAmmoInfo(CurrentAmmoData);
}

void USHGWeaponInfoUserWidget::UpdateWeaponIconImage(const FWeaponUIData& CurrentWeaponUIData)
{
    WeaponIconImage->SetBrush(FSlateImageBrush{CurrentWeaponUIData.MainIcon, GetImageSize(CurrentWeaponUIData.MainIcon), WeaponIconTint});
}

void USHGWeaponInfoUserWidget::UpdateAmmoInfo(const FAmmoData& CurrentAmmoData)
{
    const auto NewBulletsStr = ShooterUtils::FormatStringFromInt(CurrentAmmoData.BulletsInClip, AmmoInfoMinStrLen, AmmoInfoFillChar);
    BulletsTextBlock->SetText(FText::FromString(NewBulletsStr));

    auto NewClipsStr = InfiniteClipsSymbol;
    if (!CurrentAmmoData.InfiniteClips)
    {
        NewClipsStr = ShooterUtils::FormatStringFromInt(CurrentAmmoData.Clips, AmmoInfoMinStrLen, AmmoInfoFillChar);
    }
    NewClipsStr = FString::Printf(TEXT("/%s"), *NewClipsStr);
    ClipsTextBlock->SetText(FText::FromString(NewClipsStr));
}