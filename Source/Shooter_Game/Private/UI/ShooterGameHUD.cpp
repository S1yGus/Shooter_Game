// Shooter_Game, All rights reserved.

#include "UI/ShooterGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterArenaGameMode.h"

void AShooterGameHUD::BeginPlay()
{
    Super::BeginPlay();
}

void AShooterGameHUD::BackToRootMenu()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<AShooterArenaGameMode>();
    if (!GameMode)
        return;

    GameMode->InPause();
}

void AShooterGameHUD::SetupWidgets()
{
    if (PlayerGameWidgetClass)
    {
        GameWidgets.Add(EGameState::InGame, CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), PlayerGameWidgetClass));
    }
    if (PlayerSpectatingWidgetClass)
    {
        GameWidgets.Add(EGameState::InSpectating, CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), PlayerSpectatingWidgetClass));
    }
    if (PauseWidgetClass)
    {
        GameWidgets.Add(EGameState::Pause, CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), PauseWidgetClass));
    }
    if (GameOverWidgetClass)
    {
        GameWidgets.Add(EGameState::GameOver, CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), GameOverWidgetClass));
    }

    Super::SetupWidgets();
}
