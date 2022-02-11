// Shooter_Game, All rights reserved.

#include "UI/ShooterPauseUserWidget.h"
#include "UI/ShooterButtonUserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UShooterPauseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClickedButton.AddDynamic(this, &UShooterPauseUserWidget::OnClearPause);
    }
}

void UShooterPauseUserWidget::OnClearPause()
{
    const auto GameMode = GetWorld()->GetAuthGameMode();
    if (!GameMode)
        return;

    GameMode->ClearPause();
}
