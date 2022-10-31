// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGSpectatingUserWidget.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGSpectatingUserWidget : public USHGAnimatedUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FText GetCurrentRespawnTime() const;

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* BlinkingAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FText DeathText = FText::FromString("YOU DIED");

    virtual void NativeOnInitialized() override;
};
