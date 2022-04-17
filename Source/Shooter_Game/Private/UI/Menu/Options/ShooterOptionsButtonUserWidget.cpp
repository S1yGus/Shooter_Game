// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/ShooterOptionsButtonUserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UShooterOptionsButtonUserWidget::SetOptionsButtonData(const FOptionsButtonData& NewOptionsButtonData)
{
    OptionsButtonData = NewOptionsButtonData;

    if (ButtonTextBlock)
    {
        ButtonTextBlock->SetText(NewOptionsButtonData.ButtonText);
    }
}

void UShooterOptionsButtonUserWidget::SetSelected(bool IsSelected)
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UShooterOptionsButtonUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (OptionsButton)
    {
        OptionsButton->OnClicked.AddDynamic(this, &UShooterOptionsButtonUserWidget::OnClicked);
    }
}

void UShooterOptionsButtonUserWidget::OnClicked()
{
    OnClickedOptionsButton.Broadcast(OptionsButtonData);
}
