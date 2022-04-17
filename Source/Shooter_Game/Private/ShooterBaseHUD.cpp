// Shooter_Game, All rights reserved.

#include "ShooterBaseHUD.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterGameModeBase.h"

void AShooterBaseHUD::BeginPlay()
{
    Super::BeginPlay();

    SetupWidgets();

    const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
        return;

    GameMode->OnGameStateChanged.AddUObject(this, &AShooterBaseHUD::OnGameStateChanged);
}

void AShooterBaseHUD::SetupWidgets()
{
    if (OptionsWidgetClass)
    {
        GameWidgets.Add(EGameState::Options, CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), OptionsWidgetClass));
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

void AShooterBaseHUD::OnGameStateChanged(EGameState GameState)
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