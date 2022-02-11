// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterPauseUserWidget.generated.h"

class UShooterButtonUserWidget;

UCLASS()
class SHOOTER_GAME_API UShooterPauseUserWidget : public UShooterBaseAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* ClearPauseButton = nullptr;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnClearPause();
};
