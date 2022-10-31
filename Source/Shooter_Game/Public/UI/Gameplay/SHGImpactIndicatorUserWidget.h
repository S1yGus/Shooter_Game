// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGImpactIndicatorUserWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTER_GAME_API USHGImpactIndicatorUserWidget : public USHGAnimatedUserWidget
{
    GENERATED_BODY()

public:
    void SetImpactWidgetText(const FText& NewText);
    void SetImpactWidgetTextColor(const FLinearColor& NewColor);

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ImpactText;

    virtual void NativeOnInitialized() override;
};
