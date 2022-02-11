// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterBaseAnimatedUserWidget.generated.h"

class UWidgetAnimation;

UCLASS()
class SHOOTER_GAME_API UShooterBaseAnimatedUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void ShowStartupAnimation();

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* StartupAnimation = nullptr;
};
