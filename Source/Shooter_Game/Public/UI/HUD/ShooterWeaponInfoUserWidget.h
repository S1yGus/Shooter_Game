// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/ShooterWeaponInfoBaseUserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponInfoUserWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SHOOTER_GAME_API UShooterWeaponInfoUserWidget : public UShooterWeaponInfoBaseUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UImage* WeaponIconImage = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* BulletsTextBlock = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ClipsTextBlock = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FSlateColor WeaponIconTint = FSlateColor(FLinearColor::White);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FString InfiniteClipsSymbol = "Inf.";

private:
    virtual void OnNewPawn(APawn* NewPawn) override;

    void OnWeaponChanged(const FWeaponUIData& CurrentWeaponUIData, const FAmmoData& CurrentAmmoData);

    bool GetCurrentWeaponAmmoData(FAmmoData& Data) const;

    void UpdateWeaponIconImage(const FWeaponUIData& CurrentWeaponUIData);
    void UpdateAmmoInfo(const FAmmoData& CurrentAmmoData);
};
