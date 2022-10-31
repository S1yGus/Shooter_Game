// Shooter_Game, All rights reserved.

#include "UI/Menu/SHGTitleUserWidget.h"
#include "Components/TextBlock.h"

void USHGTitleUserWidget::SetText(const FText& NewText)
{
    Text = NewText;
    TextBlock->SetText(NewText);
}

void USHGTitleUserWidget::SetColor(const FLinearColor& NewColor)
{
    TextBlock->SetColorAndOpacity(FSlateColor{NewColor});
}

void USHGTitleUserWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    check(TextBlock);

    TextBlock->SetText(Text);
}
