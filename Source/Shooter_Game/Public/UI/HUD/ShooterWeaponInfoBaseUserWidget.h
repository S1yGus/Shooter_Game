// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponInfoBaseUserWidget.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterWeaponInfoBaseUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;
    virtual void OnNewPawn(APawn* NewPawn);

    FVector2D GetImageSize(UTexture2D* Image);
    bool GetCurrentWeaponUIData(FWeaponUIData& Data) const;
};
