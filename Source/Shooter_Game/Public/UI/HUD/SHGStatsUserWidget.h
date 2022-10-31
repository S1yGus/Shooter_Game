// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGStatsUserWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTER_GAME_API USHGStatsUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* NotEnoughStaminaAnimation;

    UPROPERTY(Meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    UPROPERTY(Meta = (BindWidget))
    UProgressBar* StaminaProgressBar;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Health")
    float HealthColorThreshold = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Health")
    FLinearColor NormalHealthColor{FLinearColor::White};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Health")
    FLinearColor LowHealthColor{FLinearColor::Red};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Stamina")
    float StaminaColorThreshold = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Stamina")
    FLinearColor NormalStaminaColor{FLinearColor::White};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Stamina")
    FLinearColor LowStaminaColor{FLinearColor::Red};

    virtual void NativeOnInitialized() override;

private:
    void OnNewPawn(APawn* NewPawn);
    void OnStaminaChanged(float Stamina, float StaminaPercent);
    void OnHealthChanged(float Health, float HealthPercent);
    void OnNotEnoughStamina();

    void ShowAnimation(UWidgetAnimation* Animation);

    void UpdateHealthProgressBar(float HealthPercent);
    void UpdateStaminaProgressBar(float StaminaPercent);
};
