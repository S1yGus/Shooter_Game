// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/ShooterWeaponInfoBaseUserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterCrosshairUserWidget.generated.h"

class UImage;

UCLASS()
class SHOOTER_GAME_API UShooterCrosshairUserWidget : public UShooterWeaponInfoBaseUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UImage* CrosshairImage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FSlateColor CrosshairTint = FSlateColor(FLinearColor::White);

private:
    virtual void OnNewPawn(APawn* NewPawn) override;

    void OnWeaponChanged(const FWeaponUIData& CurrentWeaponUIData, const FAmmoData& CurrentAmmoData);

    void UpdateCrosshairImage(const FWeaponUIData& CurrentWeaponUIData);
};
