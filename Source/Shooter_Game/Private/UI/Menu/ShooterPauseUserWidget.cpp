// Shooter_Game, All rights reserved.

#include "UI/Menu/ShooterPauseUserWidget.h"
#include "UI/Menu/ShooterButtonUserWidget.h"
#include "ShooterGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UShooterPauseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (!ClearPauseButton)
        return;

    ClearPauseButton->OnClickedButton.AddDynamic(this, &UShooterPauseUserWidget::OnClearPause);
    OptionsButton->OnClickedButton.AddDynamic(this, &UShooterPauseUserWidget::OnOptions);
}

void UShooterPauseUserWidget::OnClearPause()
{
    ShowFadeoutAnimation();
}

void UShooterPauseUserWidget::OnOptions()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<AShooterGameModeBase>();
    if (!GameMode)
        return;

    GameMode->InOptions();
}

void UShooterPauseUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    // clear pause
    if (Animation == FadeoutAnimation)
    {
        const auto GameMode = GetWorld()->GetAuthGameMode();
        if (!GameMode)
            return;

        if (GameMode->ClearPause())
        {
            ResetStartupAnimationFlag();
        }
    }
}
