// Shooter_Game, All rights reserved.

#include "Menu/UI/ShooterLevelItemUserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UShooterLevelItemUserWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(LevelData.LevelThumbnails);
    }

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(LevelData.LevelDisplayName);
    }
}

void UShooterLevelItemUserWidget::SetSelected(bool IsSelected)
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UShooterLevelItemUserWidget::NativeOnInitialized()
{
    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &UShooterLevelItemUserWidget::OnLevelItemClicked);
        LevelSelectButton->OnHovered.AddDynamic(this, &UShooterLevelItemUserWidget::OnLevelItemHovered);
        LevelSelectButton->OnUnhovered.AddDynamic(this, &UShooterLevelItemUserWidget::OnLevelItemUnhovered);
    }
}

void UShooterLevelItemUserWidget::OnLevelItemClicked()
{
    OnSelectedLevel.Broadcast(LevelData);
}

void UShooterLevelItemUserWidget::OnLevelItemHovered()
{
    if (LevelImage)
    {
        LevelImage->SetColorAndOpacity(FLinearColor(0.05f, 1.0f, 0.2f, 1.0f));
    }
}

void UShooterLevelItemUserWidget::OnLevelItemUnhovered()
{
    if (LevelImage)
    {
        LevelImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.7f));
    }
}
