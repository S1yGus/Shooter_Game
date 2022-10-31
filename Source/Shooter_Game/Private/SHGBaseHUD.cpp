// Shooter_Game, All rights reserved.

#include "SHGBaseHUD.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGGameModeBase.h"

void ASHGBaseHUD::BeginPlay()
{
    Super::BeginPlay();

    check(OptionsWidgetClass);
    check(OptionsWarningWidgetClass);

    SetupWidgets();

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }
}

void ASHGBaseHUD::SetupWidgets()
{
    GameWidgets.Add(EGameState::Options, CreateWidget<USHGAnimatedUserWidget>(GetWorld(), OptionsWidgetClass));
    GameWidgets.Add(EGameState::OptionsWarning, CreateWidget<USHGAnimatedUserWidget>(GetWorld(), OptionsWarningWidgetClass));

    for (const auto& [GameState, GameWidget] : GameWidgets)
    {
        if (!GameWidget)
            continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void ASHGBaseHUD::OnGameStateChanged(EGameState GameState)
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