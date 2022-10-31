// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/SHGWeaponInfoBaseUserWidget.h"
#include "ShooterCoreTypes.h"
#include "SHGWeaponInfoUserWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SHOOTER_GAME_API USHGWeaponInfoUserWidget : public USHGWeaponInfoBaseUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UImage* WeaponIconImage;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* BulletsTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ClipsTextBlock;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FSlateColor WeaponIconTint{FLinearColor::White};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FString InfiniteClipsSymbol{"Inf."};

    virtual void NativeOnInitialized() override;

private:
    virtual void OnNewPawn(APawn* NewPawn) override;

    void OnWeaponChanged(const FWeaponUIData& CurrentWeaponUIData, const FAmmoData& CurrentAmmoData);

    void UpdateWeaponIconImage(const FWeaponUIData& CurrentWeaponUIData);
    void UpdateAmmoInfo(const FAmmoData& CurrentAmmoData);
};
