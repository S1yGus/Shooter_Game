// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterOnceAnimatedUserWidget.h"
#include "ShooterPauseUserWidget.generated.h"

class UShooterButtonUserWidget;

UCLASS()
class SHOOTER_GAME_API UShooterPauseUserWidget : public UShooterOnceAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* ClearPauseButton = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* OptionsButton = nullptr;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnClearPause();

    UFUNCTION()
    void OnOptions();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
