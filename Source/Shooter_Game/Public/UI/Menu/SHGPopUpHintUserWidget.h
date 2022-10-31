// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGUICoreTypes.h"
#include "SHGPopUpHintUserWidget.generated.h"

class UTextBlock;
class USHGTitleUserWidget;
class USHGButtonUserWidget;

UCLASS()
class SHOOTER_GAME_API USHGPopUpHintUserWidget : public USHGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    USHGTitleUserWidget* TitleText;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* HintTextBlock;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* ResumeButton;

    virtual void NativeOnInitialized() override;

private:
    void OnShowPopUpHint(const FHintData& HintData);
    void OnPressedEnter();
    void OnClickedResumeButton();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
