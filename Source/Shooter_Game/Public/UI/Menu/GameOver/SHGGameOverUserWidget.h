// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGBaseCoreTypes.h"
#include "SHGGameOverUserWidget.generated.h"

class UVerticalBox;
class USHGButtonUserWidget;
class USHGPlayerInfoRowUserWidget;

UCLASS()
class SHOOTER_GAME_API USHGGameOverUserWidget : public USHGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UVerticalBox* PlayersInfoBox;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* RestartButton;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* MenuButton;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* QuitButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGPlayerInfoRowUserWidget> PlayersInfoRowWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    void OnClickedRestartButton();
    void OnClickedMenuButton();
    void OnClickedQuitButton();
    void OnGameStateChanged(EGameState GameState);

    void UpdatePlayersInfo();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
