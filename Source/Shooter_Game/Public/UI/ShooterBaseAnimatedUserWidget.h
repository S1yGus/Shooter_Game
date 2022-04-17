// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterBaseAnimatedUserWidget.generated.h"

class UWidgetAnimation;
class USoundCue;

UCLASS()
class SHOOTER_GAME_API UShooterBaseAnimatedUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void ShowStartupAnimation();
    void ShowFadeoutAnimation();

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* StartupAnimation = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* StartupSound = nullptr;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeoutAnimation = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FadeoutSound = nullptr;
};
