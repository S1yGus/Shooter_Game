// Shooter_Game, All rights reserved.

#include "Player/SHGPlayerController.h"
#include "Components/SHGRespawnComponent.h"
#include "SHGGameMode.h"

ASHGPlayerController::ASHGPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USHGRespawnComponent>("RespawnComponent");
}

void ASHGPlayerController::BeginPlay()
{
    Super::BeginPlay();

    check(RespawnComponent);

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameMode>())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ThisClass::OnGameStateChanged);
    }
}

void ASHGPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    check(InputComponent);

    InputComponent->BindAction("Enter", EInputEvent::IE_Pressed, this, &ThisClass::OnPressedEnter).bExecuteWhenPaused = true;
    InputComponent->BindAction("Esc", EInputEvent::IE_Pressed, this, &ThisClass::OnPressedEscape).bExecuteWhenPaused = true;
}

void ASHGPlayerController::OnPressedEnter()
{
    OnPressedEnt.Broadcast();
}

void ASHGPlayerController::OnPressedEscape()
{
    OnPressedEsc.Broadcast();
}

void ASHGPlayerController::OnGameStateChanged(EGameState GameState)
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
