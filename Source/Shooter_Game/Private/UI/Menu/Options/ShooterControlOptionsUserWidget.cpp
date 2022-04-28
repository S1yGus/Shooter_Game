// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/ShooterControlOptionsUserWidget.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "ShooterGameInstance.h"
#include "ShooterSettingsSave.h"
#include "Kismet/KismetMathLibrary.h"

void UShooterControlOptionsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    UpdateSettings();
    SetupSettingsComponents();
}

void UShooterControlOptionsUserWidget::SetupSettingsComponents()
{
    if (InvertMouseYCheckBox)
    {
        InvertMouseYCheckBox->OnCheckStateChanged.AddDynamic(this, &UShooterControlOptionsUserWidget::OnCheckStateChangedInvertMouseY);
    }

    if (MouseYSensSlider)
    {
        MouseYSensSlider->OnMouseCaptureEnd.AddDynamic(this, &UShooterControlOptionsUserWidget::OnMouseCaptureEndMouseYSens);
    }

    if (MouseXSensSlider)
    {
        MouseXSensSlider->OnMouseCaptureEnd.AddDynamic(this, &UShooterControlOptionsUserWidget::OnMouseCaptureEndMouseXSens);
    }

    if (MouseAimedYSensSlider)
    {
        MouseAimedYSensSlider->OnMouseCaptureEnd.AddDynamic(this, &UShooterControlOptionsUserWidget::OnMouseCaptureEndMouseAimedYSens);
    }

    if (MouseAimedXSensSlider)
    {
        MouseAimedXSensSlider->OnMouseCaptureEnd.AddDynamic(this, &UShooterControlOptionsUserWidget::OnMouseCaptureEndMouseAimedXSens);
    }
}

UShooterSettingsSave* UShooterControlOptionsUserWidget::GetSettingsSave()
{
    const auto GameInstance = GetWorld()->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance)
        return nullptr;

    return GameInstance->GetSettingsSave();
}

void UShooterControlOptionsUserWidget::UpdateSettings()
{
    UpdateInvertMouseYCheckBox();
    UpdateMouseYSensSlider();
    UpdateMouseXSensSlider();
    UpdateMouseAimedYSensSlider();
    UpdateMouseAimedXSensSlider();
}

void UShooterControlOptionsUserWidget::UpdateInvertMouseYCheckBox()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    const auto ControlSettings = SettingsSave->GetControlSettings();
    InvertMouseYCheckBox->SetIsChecked(ControlSettings.MouseYSens < 0 && ControlSettings.MouseAimedYSens < 0);
}

void UShooterControlOptionsUserWidget::UpdateMouseYSensSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    const auto YSens = SettingsSave->GetControlSettings().MouseYSens;
    MouseYSensSlider->SetValue(UKismetMathLibrary::NormalizeToRange(FMath::Abs(YSens), MouseSensRange.X, MouseSensRange.Y));
}

void UShooterControlOptionsUserWidget::UpdateMouseXSensSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    const auto XSens = SettingsSave->GetControlSettings().MouseXSens;
    MouseXSensSlider->SetValue(UKismetMathLibrary::NormalizeToRange(XSens, MouseSensRange.X, MouseSensRange.Y));
}

void UShooterControlOptionsUserWidget::UpdateMouseAimedYSensSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    const auto AimedYSens = SettingsSave->GetControlSettings().MouseAimedYSens;
    MouseAimedYSensSlider->SetValue(UKismetMathLibrary::NormalizeToRange(FMath::Abs(AimedYSens), MouseSensRange.X, MouseSensRange.Y));
}

void UShooterControlOptionsUserWidget::UpdateMouseAimedXSensSlider()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    const auto AimedXSens = SettingsSave->GetControlSettings().MouseAimedXSens;
    MouseAimedXSensSlider->SetValue(UKismetMathLibrary::NormalizeToRange(AimedXSens, MouseSensRange.X, MouseSensRange.Y));
}

void UShooterControlOptionsUserWidget::OnCheckStateChangedInvertMouseY(bool bIsChecked)
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto ControlSettings = SettingsSave->GetControlSettings();
    if (bIsChecked && ControlSettings.MouseYSens > 0 || !bIsChecked && ControlSettings.MouseYSens < 0)
    {
        ControlSettings.MouseYSens *= -1.0f;
    }
    if (bIsChecked && ControlSettings.MouseAimedYSens > 0 || !bIsChecked && ControlSettings.MouseAimedYSens < 0)
    {
        ControlSettings.MouseAimedYSens *= -1.0f;
    }
    SettingsSave->SetControlSettings(ControlSettings);
}

void UShooterControlOptionsUserWidget::OnMouseCaptureEndMouseYSens()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto ControlSettings = SettingsSave->GetControlSettings();
    const auto InvertMultiplier = ControlSettings.MouseYSens < 0 ? -1.0f : 1.0f;
    ControlSettings.MouseYSens =
        FMath::GetMappedRangeValueClamped(FVector2d(0.0f, 1.0f), MouseSensRange, MouseYSensSlider->GetNormalizedValue()) * InvertMultiplier;
    SettingsSave->SetControlSettings(ControlSettings);
}

void UShooterControlOptionsUserWidget::OnMouseCaptureEndMouseXSens()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto ControlSettings = SettingsSave->GetControlSettings();
    ControlSettings.MouseXSens = FMath::GetMappedRangeValueClamped(FVector2d(0.0f, 1.0f), MouseSensRange, MouseXSensSlider->GetNormalizedValue());
    SettingsSave->SetControlSettings(ControlSettings);
}

void UShooterControlOptionsUserWidget::OnMouseCaptureEndMouseAimedYSens()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto ControlSettings = SettingsSave->GetControlSettings();
    const auto InvertMultiplier = ControlSettings.MouseAimedYSens < 0 ? -1.0f : 1.0f;
    ControlSettings.MouseAimedYSens =
        FMath::GetMappedRangeValueClamped(FVector2d(0.0f, 1.0f), MouseSensRange, MouseAimedYSensSlider->GetNormalizedValue()) * InvertMultiplier;
    SettingsSave->SetControlSettings(ControlSettings);
}

void UShooterControlOptionsUserWidget::OnMouseCaptureEndMouseAimedXSens()
{
    const auto SettingsSave = GetSettingsSave();
    if (!SettingsSave)
        return;

    auto ControlSettings = SettingsSave->GetControlSettings();
    ControlSettings.MouseAimedXSens = FMath::GetMappedRangeValueClamped(FVector2d(0.0f, 1.0f), MouseSensRange, MouseAimedXSensSlider->GetNormalizedValue());
    SettingsSave->SetControlSettings(ControlSettings);
}
