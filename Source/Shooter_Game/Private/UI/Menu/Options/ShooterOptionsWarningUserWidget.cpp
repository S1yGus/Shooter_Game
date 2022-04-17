// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/ShooterOptionsWarningUserWidget.h"
#include "UI/Menu/ShooterButtonUserWidget.h"
#include "GameFramework/GameUserSettings.h"

void UShooterOptionsWarningUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SaveButton)
    {
        SaveButton->OnClickedButton.AddDynamic(this, &UShooterOptionsWarningUserWidget::OnSaveSettings);
    }

    if (CancelButton)
    {
        CancelButton->OnClickedButton.AddDynamic(this, &UShooterOptionsWarningUserWidget::OnCancelSettings);
    }
}

void UShooterOptionsWarningUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    CancelSettingsTime -= InDeltaTime;
    if (CancelSettingsTime <= 0.0f)
    {
        OnCancelSettings();
    }
}

void UShooterOptionsWarningUserWidget::OnSaveSettings()
{
    GEngine->GetGameUserSettings()->SaveSettings();
    this->RemoveFromParent();
}

void UShooterOptionsWarningUserWidget::OnCancelSettings()
{
    GEngine->GetGameUserSettings()->LoadSettings();
    GEngine->GetGameUserSettings()->ApplyResolutionSettings(false);
    OnCancelResolutionSettings.Broadcast();
    this->RemoveFromParent();
}
