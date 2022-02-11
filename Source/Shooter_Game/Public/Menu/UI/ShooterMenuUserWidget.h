// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterMenuUserWidget.generated.h"

class UShooterButtonUserWidget;
class UWidgetAnimation;

UCLASS()
class SHOOTER_GAME_API UShooterMenuUserWidget : public UShooterBaseAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* LoadingAnimation = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* StartNewGameButton = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* QuitButton = nullptr;

    virtual void NativeOnInitialized() override;

protected:
    UFUNCTION()
    void OnStartNewGame();

    UFUNCTION()
    void OnQuit();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
