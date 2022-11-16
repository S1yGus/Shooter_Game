// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGPauseUserWidget.generated.h"

class USHGButtonUserWidget;

UCLASS()
class SHOOTER_GAME_API USHGPauseUserWidget : public USHGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* ResumeButton;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* OptionsButton;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* MenuButton;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* QuitButton;

    virtual void NativeOnInitialized() override;

private:
    void OnPressedEsc();
    void OnClickedResumeButton();
    void OnClickedOptionsButton();
    void OnClickedMenuButton();
    void OnClickedQuitButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
