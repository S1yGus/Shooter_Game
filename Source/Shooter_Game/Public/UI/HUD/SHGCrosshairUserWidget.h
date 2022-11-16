// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/SHGWeaponInfoBaseUserWidget.h"
#include "SHGUICoreTypes.h"
#include "SHGWeaponCoreTypes.h"
#include "SHGCrosshairUserWidget.generated.h"

class UImage;

UCLASS()
class SHOOTER_GAME_API USHGCrosshairUserWidget : public USHGWeaponInfoBaseUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UImage* CrosshairImage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FSlateColor CrosshairTint{FLinearColor::White};

    virtual void NativeOnInitialized() override;

private:
    virtual void OnNewPawn(APawn* NewPawn) override;

    void OnWeaponChanged(const FWeaponUIData& CurrentWeaponUIData, const FAmmoData& CurrentAmmoData);

    void UpdateCrosshairImage(const FWeaponUIData& CurrentWeaponUIData);
};
