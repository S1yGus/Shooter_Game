// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/SHGSettingWidgetInterface.h"
#include "SHGSliderSettingUserWidget.generated.h"

class USlider;
class USHGFloatSetting;
class UTextBlock;

UCLASS()
class SHOOTER_GAME_API USHGSliderSettingUserWidget : public UUserWidget, public ISHGSettingWidgetInterface
{
    GENERATED_BODY()

public:
    void Init(USHGFloatSetting* NewSetting);
    void UpdatePercentText();

    virtual void Update() override;

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* SettingNameText;

    UPROPERTY(Meta = (BindWidget))
    USlider* SettingSlider;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* PercentText;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    USHGFloatSetting* Setting;

    UFUNCTION()
    void OnValueChanged(float Value);
    UFUNCTION()
    void OnMouseCaptureEnd();
};
