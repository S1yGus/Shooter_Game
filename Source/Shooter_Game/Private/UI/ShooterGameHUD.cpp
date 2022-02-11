// Shooter_Game, All rights reserved.

#include "UI/ShooterGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterGameModeBase.h"

void AShooterGameHUD::BeginPlay()
{
    Super::BeginPlay();

    InitializeWidgets();

    const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
        return;

    GameMode->OnGameStateChanged.AddUObject(this, &AShooterGameHUD::OnGameStateChanged);
}

// Debug
void AShooterGameHUD::DrawCrossHair()
{
    FVector2D Center = FVector2D(Canvas->SizeX / 2, Canvas->SizeY / 2);
    int32 HalfLineSize = 10;
    int32 LineThicknes = 3;
    FLinearColor CrossColor = FLinearColor::Yellow;

    DrawLine(Center.X - HalfLineSize, Center.Y, Center.X + HalfLineSize, Center.Y, CrossColor, LineThicknes);
    DrawLine(Center.X, Center.Y - HalfLineSize, Center.X, Center.Y + HalfLineSize, CrossColor, LineThicknes);
}

void AShooterGameHUD::InitializeWidgets()
{
    if (PlayerHUDWidgetClass)
    {
        GameWidgets.Add(EGameState::InProgress, CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    }
    if (PauseWidgetClass)
    {
        GameWidgets.Add(EGameState::Pause, CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), PauseWidgetClass));
    }
    if (GameOverWidgetClass)
    {
        GameWidgets.Add(EGameState::GameOver, CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), GameOverWidgetClass));
    }

    for (const auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget)
            continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void AShooterGameHUD::OnGameStateChanged(EGameState GameState)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (GameWidgets.Contains(GameState))
    {
        CurrentWidget = GameWidgets[GameState];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->ShowStartupAnimation();
    }
}
