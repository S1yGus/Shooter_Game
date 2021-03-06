// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterHealthBarUserWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTER_GAME_API UShooterHealthBarUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetHealthPercent(float NewHealthPercent);

protected:
    UPROPERTY(Meta = (BindWidget))
    UProgressBar* HealthProgressBar = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float HealthPercentVisibilityThreshold = 0.99f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float HealthColorThreshold = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor NormalHealthColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor LowHealthColor = FLinearColor::Red;
};
