// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterHUDUserWidget.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterHUDUserWidget : public UShooterBaseAnimatedUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool IsPlayerSpectating() const;
};
