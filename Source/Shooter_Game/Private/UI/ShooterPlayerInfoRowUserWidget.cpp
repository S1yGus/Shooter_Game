// Shooter_Game, All rights reserved.

#include "UI/ShooterPlayerInfoRowUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UShooterPlayerInfoRowUserWidget::SetNameTextBlock(const FText& NewText)
{
    if (!NameTextBlock)
        return;

    NameTextBlock->SetText(NewText);
}

void UShooterPlayerInfoRowUserWidget::SetKillsTextBlock(const FText& NewText)
{
    if (!KillsTextBlock)
        return;

    KillsTextBlock->SetText(NewText);
}

void UShooterPlayerInfoRowUserWidget::SetDeathsTextBlock(const FText& NewText)
{
    if (!DeathsTextBlock)
        return;

    DeathsTextBlock->SetText(NewText);
}

void UShooterPlayerInfoRowUserWidget::SetTeamTextBlock(const FText& NewText)
{
    if (!TeamTextBlock)
        return;

    TeamTextBlock->SetText(NewText);
}

void UShooterPlayerInfoRowUserWidget::SetTeamColorImage(const FLinearColor& NewColor)
{
    if (!TeamColorImage)
        return;

    TeamColorImage->SetColorAndOpacity(NewColor);
}

void UShooterPlayerInfoRowUserWidget::SetPlayerHighlighting(bool IsHighlighting)
{
    if (!PlayerHighlighting)
        return;

    PlayerHighlighting->SetVisibility(IsHighlighting ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
