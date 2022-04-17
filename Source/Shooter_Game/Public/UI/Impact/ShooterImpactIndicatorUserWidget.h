// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterImpactIndicatorUserWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTER_GAME_API UShooterImpactIndicatorUserWidget : public UShooterBaseAnimatedUserWidget
{
    GENERATED_BODY()

public:
    void SetImpactWidgetText(const FText& NewText);
    void SetImpactWidgetTextColor(const FLinearColor& NewColor);

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ImpactText = nullptr;
};
