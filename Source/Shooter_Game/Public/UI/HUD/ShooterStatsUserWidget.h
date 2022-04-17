// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterStatsUserWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTER_GAME_API UShooterStatsUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* NotEnoughStaminaAnimation = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UProgressBar* HealthProgressBar = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UProgressBar* StaminaProgressBar = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Health")
    float HealthColorThreshold = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Health")
    FLinearColor NormalHealthColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Health")
    FLinearColor LowHealthColor = FLinearColor::Red;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Stamina")
    float StaminaColorThreshold = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Stamina")
    FLinearColor NormalStaminaColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Stamina")
    FLinearColor LowStaminaColor = FLinearColor::Red;

    virtual void NativeOnInitialized() override;

private:
    void OnNewPawn(APawn* NewPawn);
    void OnStaminaChanged(float Stamina, float StaminaPercent);
    void OnHealthChanged(float Health, float HealthPercent);
    void OnNotEnoughStamina();
    void OnOutOfStamina();

    void ShowAnimation(UWidgetAnimation* Animation);
    float GetHelthPercent() const;
    float GetStaminaPercent() const;

    void UpdateHealthProgressBar(const float HealthPercent);
    void UpdateStaminaProgressBar(const float StaminaPercent);
};
