// Shooter_Game, All rights reserved.

#include "Player/ShooterPlayerController.h"
#include "Components/ShooterRespawnComponent.h"
#include "SHGGameModeBase.h"

AShooterPlayerController::AShooterPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<UShooterRespawnComponent>("RespawnComponent");
}

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    const auto GameMode = Cast<ASHGGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
        return;

    GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
}

void AShooterPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    check(InputComponent);

    InputComponent->BindAction("Enter", EInputEvent::IE_Pressed, this, &ThisClass::OnPressedEnter).bExecuteWhenPaused = true;
    InputComponent->BindAction("Esc", EInputEvent::IE_Pressed, this, &ThisClass::OnPressedEscape).bExecuteWhenPaused = true;
}

void AShooterPlayerController::OnPressedEnter()
{
    OnPressedEnt.Broadcast();
}

void AShooterPlayerController::OnPressedEscape()
{
    OnPressedEsc.Broadcast();
}

void AShooterPlayerController::OnGameStateChanged(EGameState GameState)
{
    if (GameState == EGameState::Game || GameState == EGameState::Spectating)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false));
        bShowMouseCursor = true;
    }
}
