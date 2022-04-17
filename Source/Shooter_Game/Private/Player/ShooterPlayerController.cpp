// Shooter_Game, All rights reserved.

#include "Player/ShooterPlayerController.h"
#include "Components/ShooterRespawnComponent.h"
#include "ShooterGameModeBase.h"

AShooterPlayerController::AShooterPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<UShooterRespawnComponent>("RespawnComponent");
}

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
        return;

    GameMode->OnGameStateChanged.AddUObject(this, &AShooterPlayerController::OnGameStateChanged);
}

void AShooterPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent)
        return;

    InputComponent->BindAction("Pause", IE_Pressed, this, &AShooterPlayerController::OnSetPause);
}

void AShooterPlayerController::OnSetPause()
{
    const auto GameMode = GetWorld()->GetAuthGameMode();
    if (!GameMode)
        return;

    GameMode->SetPause(this);
}

void AShooterPlayerController::OnGameStateChanged(EGameState GameState)
{
    if (GameState == EGameState::InGame || GameState == EGameState::InSpectating)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}
