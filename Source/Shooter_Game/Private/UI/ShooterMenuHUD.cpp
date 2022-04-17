// Shooter_Game, All rights reserved.

#include "UI/ShooterMenuHUD.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "Menu/ShooterMenuGameMode.h"

void AShooterMenuHUD::BeginPlay()
{
    Super::BeginPlay();
}

void AShooterMenuHUD::BackToRootMenu()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<AShooterMenuGameMode>();
    if (!GameMode)
        return;

    GameMode->InMainMenu();
}

void AShooterMenuHUD::SetupWidgets()
{
    if (MainMenuWidgetClass)
    {
        GameWidgets.Add(EGameState::MainMenu, CreateWidget<UShooterBaseAnimatedUserWidget>(GetWorld(), MainMenuWidgetClass));
    }

    Super::SetupWidgets();
}
