// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGBaseCoreTypes.h"
#include "SHGOptionsMenuUserWidget.generated.h"

class USHGButtonUserWidget;
class USHGOptionsUserWidget;
class USHGOptionsButtonUserWidget;
class UWidgetSwitcher;
class UVerticalBox;

UCLASS()
class SHOOTER_GAME_API USHGOptionsMenuUserWidget : public USHGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGOptionsUserWidget> OptionsWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGOptionsButtonUserWidget> OptionsButtonClass;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* SwitchInAnimation;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* SwitchOutAnimation;

    UPROPERTY(Meta = (BindWidget))
    UVerticalBox* OptionsButtonsBox;

    UPROPERTY(Meta = (BindWidget))
    UWidgetSwitcher* OptionsWidgetSwitcher;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* BackButton;

    virtual void NativeOnInitialized() override;

private:
    int8 WidgetIDToSet = 0;

    inline void BackToRootMenu();
    void FillOptionsButtonsBox();
    void SelectOptionsCategory();

    void OnPressedEscape();
    void OnClickedBackButton();
    void OnResolutionChanged();
    void OnSelectedOptionsCategory(int32 WidgetID);
    void OnGameStateChanged(EGameState GameState);

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
