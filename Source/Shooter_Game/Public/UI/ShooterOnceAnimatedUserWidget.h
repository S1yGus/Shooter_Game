// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterOnceAnimatedUserWidget.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterOnceAnimatedUserWidget : public UShooterBaseAnimatedUserWidget
{
    GENERATED_BODY()
public:
    virtual void ShowStartupAnimation() override;
    void ResetStartupAnimationFlag() { CanShowStartupAnimation = true; }

private:
    bool CanShowStartupAnimation = true;
};
