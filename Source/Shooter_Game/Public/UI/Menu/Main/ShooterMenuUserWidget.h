// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterOnceAnimatedUserWidget.h"
#include "ShooterMenuUserWidget.generated.h"

class UShooterButtonUserWidget;
class UWidgetAnimation;
class USoundCue;

UCLASS()
class SHOOTER_GAME_API UShooterMenuUserWidget : public UShooterOnceAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* StartNewGameButton = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* OptionsButton = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* QuitButton = nullptr;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnStartNewGame();

    UFUNCTION()
    void OnOptions();

    UFUNCTION()
    void OnQuit();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
