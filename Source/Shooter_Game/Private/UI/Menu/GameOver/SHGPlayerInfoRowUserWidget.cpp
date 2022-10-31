// Shooter_Game, All rights reserved.

#include "UI/Menu/GameOver/SHGPlayerInfoRowUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USHGPlayerInfoRowUserWidget::SetName(const FText& NewText)
{
    NameTextBlock->SetText(NewText);
}

void USHGPlayerInfoRowUserWidget::SetKills(const FText& NewText)
{
    KillsTextBlock->SetText(NewText);
}

void USHGPlayerInfoRowUserWidget::SetDeaths(const FText& NewText)
{
    DeathsTextBlock->SetText(NewText);
}

void USHGPlayerInfoRowUserWidget::SetTeam(const FText& NewText)
{
    TeamTextBlock->SetText(NewText);
}

void USHGPlayerInfoRowUserWidget::SetTeamColor(const FLinearColor& NewColor)
{
    TeamColorImage->SetColorAndOpacity(NewColor);
}

void USHGPlayerInfoRowUserWidget::SetPlayerHighlighting(bool IsHighlighting)
{
    PlayerHighlighting->SetVisibility(IsHighlighting ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USHGPlayerInfoRowUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(NameTextBlock);
    check(KillsTextBlock);
    check(DeathsTextBlock);
    check(TeamTextBlock);
    check(PlayerHighlighting);
    check(TeamColorImage);
}
