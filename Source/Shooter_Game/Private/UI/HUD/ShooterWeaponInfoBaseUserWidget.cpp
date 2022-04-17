// Shooter_Game, All rights reserved.

#include "UI/HUD/ShooterWeaponInfoBaseUserWidget.h"
#include "Components/ShooterWeaponComponent.h"

void UShooterWeaponInfoBaseUserWidget::NativeOnInitialized()
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
        return;

    Controller->GetOnNewPawnNotifier().AddUObject(this, &UShooterWeaponInfoBaseUserWidget::OnNewPawn);
}

FVector2D UShooterWeaponInfoBaseUserWidget::GetImageSize(UTexture2D* Image)
{
    return FVector2D(Image->GetSizeX(), Image->GetSizeY());
}

bool UShooterWeaponInfoBaseUserWidget::GetCurrentWeaponUIData(FWeaponUIData& Data) const
{
    if (!GetOwningPlayerPawn())
        return false;

    const auto WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetCurrentWeaponUIData(Data);
}

void UShooterWeaponInfoBaseUserWidget::OnNewPawn(APawn* NewPawn)
{
}
