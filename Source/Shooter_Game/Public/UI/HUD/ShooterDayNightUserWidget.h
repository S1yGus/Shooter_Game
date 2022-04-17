// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterDayNightUserWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTER_GAME_API UShooterDayNightUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FText GetCurrentTime() const;

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* DayNightTextBlock = nullptr;

    virtual void NativeOnInitialized() override;
};
