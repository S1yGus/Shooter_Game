// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGBaseCoreTypes.h"
#include "SHGAnimatedUserWidget.generated.h"

class UWidgetAnimation;
class USoundCue;

UCLASS()
class SHOOTER_GAME_API USHGAnimatedUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void ShowStartupAnimation();
    void ShowFadeoutAnimation();

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* StartupAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* StartupSound;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeoutAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FadeoutSound;

    EGameState GameStateToSet = EGameState::WaitingToStart;

    void ChangeGameState(EGameState NewGameState);
};
