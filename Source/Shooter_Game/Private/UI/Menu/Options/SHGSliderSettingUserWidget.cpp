// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/SHGSliderSettingUserWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Settings/SHGFloatSetting.h"

void USHGSliderSettingUserWidget::Init(USHGFloatSetting* NewSetting)
{
    if (!NewSetting)
        return;

    Setting = NewSetting;
    SettingNameText->SetText(Setting->GetName());

    Update();
}

void USHGSliderSettingUserWidget::UpdatePercentText()
{
    const auto PercentStr = FString::Printf(TEXT("%.0f%%"), SettingSlider->GetNormalizedValue() * 100.0f);    // * 100.0f for 100%.
    PercentText->SetText(FText::FromString(PercentStr));
}

void USHGSliderSettingUserWidget::Update()
{
    if (!Setting)
        return;

    SettingSlider->SetValue(Setting->GetCurrentValue());

    UpdatePercentText();
}

void USHGSliderSettingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingNameText);
    check(SettingSlider);
    check(PercentText);

    SettingSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnValueChanged);
    SettingSlider->OnMouseCaptureEnd.AddDynamic(this, &ThisClass::OnMouseCaptureEnd);
}

void USHGSliderSettingUserWidget::OnValueChanged(float Value)
{
    UpdatePercentText();
}

void USHGSliderSettingUserWidget::OnMouseCaptureEnd()
{
    if (!Setting)
        return;

    Setting->SetValue(SettingSlider->GetNormalizedValue());
}
