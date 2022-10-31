// Shooter_Game, All rights reserved.

#include "UI/Menu/Options/SHGOptionsButtonUserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USHGOptionsButtonUserWidget::Init(const FText& OptionDisplayName, int32 InWidgetID)
{
    ButtonTextBlock->SetText(OptionDisplayName);
    WidgetID = InWidgetID;
}

void USHGOptionsButtonUserWidget::SetSelected(bool bIsSelected)
{
    FrameImage->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USHGOptionsButtonUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(OptionsButton);
    check(FrameImage);
    check(ButtonTextBlock);

    OptionsButton->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
}

void USHGOptionsButtonUserWidget::OnClicked()
{
    OnClickedOptionsButton.Broadcast(WidgetID);
}
