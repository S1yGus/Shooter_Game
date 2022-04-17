// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterSpectatingUserWidget.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterSpectatingUserWidget : public UShooterBaseAnimatedUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FText GetCurrentRespawnTime() const;

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* BlinkingAnimation = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FString DeathText = "YOU DIED";

    virtual void NativeOnInitialized() override;
};
