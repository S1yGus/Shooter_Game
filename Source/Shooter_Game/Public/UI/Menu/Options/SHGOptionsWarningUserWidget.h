// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGBaseCoreTypes.h"
#include "SHGOptionsWarningUserWidget.generated.h"

class USHGButtonUserWidget;

UCLASS()
class SHOOTER_GAME_API USHGOptionsWarningUserWidget : public USHGAnimatedUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FText GetCountdownTime() const;

protected:
    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* SaveButton;

    UPROPERTY(Meta = (BindWidget))
    USHGButtonUserWidget* CancelButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
    float CancelSettingsTime = 10.0f;

    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    float CountdownTime = 0.0f;

    void ResetWidget();

    void OnGameStateChanged(EGameState NewGameState);
    void OnPressedEnter();
    void OnPressedEsc();
    void OnSaveSettings();
    void OnCancelSettings();

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
