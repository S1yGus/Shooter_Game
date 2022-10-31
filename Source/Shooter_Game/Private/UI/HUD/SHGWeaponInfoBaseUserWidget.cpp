// Shooter_Game, All rights reserved.

#include "UI/HUD/SHGWeaponInfoBaseUserWidget.h"

void USHGWeaponInfoBaseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (const auto PC = GetOwningPlayer())
    {
        PC->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawn);
    }
}

FVector2D USHGWeaponInfoBaseUserWidget::GetImageSize(UTexture2D* Image) const
{
    return FVector2D{static_cast<double>(Image->GetSizeX()), static_cast<double>(Image->GetSizeY())};
}

void USHGWeaponInfoBaseUserWidget::OnNewPawn(APawn* NewPawn)
{
}
