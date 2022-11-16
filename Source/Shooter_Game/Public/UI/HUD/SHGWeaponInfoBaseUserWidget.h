// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGWeaponInfoBaseUserWidget.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGWeaponInfoBaseUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;
    virtual void OnNewPawn(APawn* NewPawn);

    FVector2D GetImageSize(UTexture2D* Image) const;
};
