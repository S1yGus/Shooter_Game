// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterSpectatingUserWidget.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterSpectatingUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool GetCurrentRespawnTime(float& RespawnTime) const;
};
