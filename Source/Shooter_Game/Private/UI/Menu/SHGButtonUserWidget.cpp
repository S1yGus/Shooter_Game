// Shooter_Game, All rights reserved.

#include "UI/Menu/SHGButtonUserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USHGButtonUserWidget::SetText(const FText& NewText)
{
    ButtonText = NewText;
    ButtonTextBlock->SetText(ButtonText);
}

void USHGButtonUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(Button);
    check(ButtonTextBlock);

    Button->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
}

void USHGButtonUserWidget::NativePreConstruct()
{
    ButtonTextBlock->SetText(ButtonText);
}

void USHGButtonUserWidget::OnClicked()
{
    OnClickedButton.Broadcast();
}
