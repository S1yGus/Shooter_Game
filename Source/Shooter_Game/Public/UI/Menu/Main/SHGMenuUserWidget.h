// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGMenuUserWidget.generated.h"

class USHGButtonUserWidget;
class UWidgetAnimation;

UCLASS()
class SHOOTER_GAME_API USHGMenuUserWidget : public USHGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* LoadingAnimation;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* NewGameButton;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* OptionsButton;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* QuitButton;

    virtual void NativeOnInitialized() override;

private:
    void OnStartNewGame();
    void OnOptions();
    void OnQuit();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
