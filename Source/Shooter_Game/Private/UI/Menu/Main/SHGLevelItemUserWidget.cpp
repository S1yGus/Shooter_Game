// Shooter_Game, All rights reserved.

#include "UI/Menu/Main/SHGLevelItemUserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USHGLevelItemUserWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    LevelImage->SetBrushFromTexture(LevelData.LevelThumbnails);
    LevelNameTextBlock->SetText(LevelData.LevelDisplayName);
}

void USHGLevelItemUserWidget::SetSelected(bool IsSelected)
{
    FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USHGLevelItemUserWidget::NativeOnInitialized()
{
    check(LevelSelectButton);
    check(LevelImage);
    check(FrameImage);
    check(LevelNameTextBlock);

    LevelSelectButton->OnClicked.AddDynamic(this, &ThisClass::OnLevelItemClicked);
    LevelSelectButton->OnHovered.AddDynamic(this, &ThisClass::OnLevelItemHovered);
    LevelSelectButton->OnUnhovered.AddDynamic(this, &ThisClass::OnLevelItemUnhovered);
}

void USHGLevelItemUserWidget::OnLevelItemClicked()
{
    OnSelectedLevel.Broadcast(LevelData);
}

void USHGLevelItemUserWidget::OnLevelItemHovered()
{
    LevelImage->SetColorAndOpacity(HoveredTint);
}

void USHGLevelItemUserWidget::OnLevelItemUnhovered()
{
    LevelImage->SetColorAndOpacity(UnhoveredTint);
}
