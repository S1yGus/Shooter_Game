// Shooter_Game, All rights reserved.

#include "UI/Menu/ShooterButtonUserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UShooterButtonUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UShooterButtonUserWidget::OnClicked);
    }
}

void UShooterButtonUserWidget::NativePreConstruct()
{
    if (ButtonTextBlock)
    {
        ButtonTextBlock->SetText(ButtonText);
    }
}

void UShooterButtonUserWidget::OnClicked()
{
    OnClickedButton.Broadcast();
}
