// Shooter_Game, All rights reserved.

#include "UI/ShooterButtonUserWidget.h"
#include "Components/Button.h"

void UShooterButtonUserWidget::NativeOnInitialized()
{
    if (!Button)
        return;

    Button->OnClicked.AddDynamic(this, &UShooterButtonUserWidget::OnClicked);
}

void UShooterButtonUserWidget::OnClicked()
{
    OnClickedButton.Broadcast();
}
