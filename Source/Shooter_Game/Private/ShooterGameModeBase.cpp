// Shooter_Game, All rights reserved.

#include "ShooterGameModeBase.h"
#include "Player/ShooterPlayerCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "UI/ShooterGameHUD.h"
#include "AIController.h"
#include "Player/ShooterPlayerState.h"
#include "Components/ShooterRespawnComponent.h"
#include "EngineUtils.h"
#include "Components/ShooterAIWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(GameModeLog, All, All)

constexpr static int32 MinRaundTimeForRespawn = 1;

AShooterGameModeBase::AShooterGameModeBase()
{
    DefaultPawnClass = AShooterPlayerCharacter::StaticClass();
    PlayerControllerClass = AShooterPlayerController::StaticClass();
    HUDClass = AShooterGameHUD::StaticClass();
    PlayerStateClass = AShooterPlayerState::StaticClass();
}

void AShooterGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnAIControllers();
    SetTeamsID();
    StartRaund();

    SetGameState(EGameState::InProgress);
}

UClass* AShooterGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
        return AIDefaultPawnClass;

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

float AShooterGameModeBase::GetCurrentRaundTime() const
{
    if (!GetWorldTimerManager().IsTimerActive(RaundTimerHandle))
        return 0.0f;

    return GetWorldTimerManager().GetTimerRemaining(RaundTimerHandle);
}

void AShooterGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    if (!KillerController || !VictimController)
        return;

    const auto KillerPlayerState = Cast<AShooterPlayerState>(KillerController->PlayerState);
    const auto VictimPlayerState = Cast<AShooterPlayerState>(VictimController->PlayerState);
    if (!KillerPlayerState || !VictimPlayerState)
        return;

    KillerPlayerState->AddKill();
    VictimPlayerState->AddDeath();

    Respawn(VictimController);
}

void AShooterGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

bool AShooterGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const bool PauseSeted = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSeted)
    {
        StopAllFire();
        SetGameState(EGameState::Pause);
    }

    return PauseSeted;
}

bool AShooterGameModeBase::ClearPause()
{
    const bool PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetGameState(EGameState::InProgress);
    }

    return PauseCleared;
}

void AShooterGameModeBase::SetTeamsID()
{
    int32 BotID = 0;
    int32 TeamID = 0;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller)
            continue;

        const auto ShooterPlayerState = Cast<AShooterPlayerState>(Controller->PlayerState);
        if (!ShooterPlayerState)
            continue;

        ShooterPlayerState->SetTeamID(TeamID);
        ShooterPlayerState->SetTeamColor(DetermenColorByTeamID(TeamID));
        TeamID = (TeamID + 1) % GameData.TeamsNum;

        ShooterPlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot_" + FString::FromInt(BotID++));
    }
}

FLinearColor AShooterGameModeBase::DetermenColorByTeamID(int32 TeamID) const
{
    FLinearColor TeamColor = GameData.DefaultTeamColor;
    if (TeamID <= GameData.TeamsColors.Num() - 1)
        TeamColor = GameData.TeamsColors[TeamID];

    return TeamColor;
}

void AShooterGameModeBase::SetPlayerColor(AController* Controller)
{
    const auto ShooterPlayerState = Cast<AShooterPlayerState>(Controller->PlayerState);
    const auto ShooterBaseCharacter = Cast<AShooterBaseCharacter>(Controller->GetPawn());
    if (!ShooterPlayerState || !ShooterBaseCharacter)
        return;

    ShooterBaseCharacter->SetColor(DetermenColorByTeamID(ShooterPlayerState->GetTeamID()));
}

void AShooterGameModeBase::SpawnAIControllers()
{
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
    }
}

void AShooterGameModeBase::StartRaund()
{
    ResetPlayers();

    GetWorldTimerManager().SetTimer(RaundTimerHandle, this, &AShooterGameModeBase::EndRaund, GameData.RaundTime);
}

void AShooterGameModeBase::EndRaund()
{
    if (CurrentRaund + 1 <= GameData.RaundsNum)
    {
        ++CurrentRaund;
        StartRaund();
    }
    else
    {
        GameOver();
    }
}

void AShooterGameModeBase::ResetPlayers()
{
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void AShooterGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (!Controller)
        return;

    const auto ControllerPawn = Controller->GetPawn();
    if (ControllerPawn)
    {
        ControllerPawn->Reset();
    }

    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void AShooterGameModeBase::Respawn(AController* Controller)
{
    if (!Controller || GetCurrentRaundTime() < MinRaundTimeForRespawn + GameData.RespawnTime)
        return;

    const auto RespawnComponent = Controller->FindComponentByClass<UShooterRespawnComponent>();
    if (!RespawnComponent)
        return;

    RespawnComponent->StartRespawn(GameData.RespawnTime);
}

void AShooterGameModeBase::GameOver()
{
    for (auto Controller : TActorRange<AController>(GetWorld()))
    {
        if (!Controller)
            continue;

        const auto Pawn = Controller->GetPawn();
        if (!Pawn)
            continue;

        Pawn->TurnOff();
        Pawn->DisableInput(nullptr);
    }

    SetGameState(EGameState::GameOver);
}

void AShooterGameModeBase::LogGameInfo()
{
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller)
            continue;

        const auto PlayerState = Cast<AShooterPlayerState>(Controller->PlayerState);
        if (!PlayerState)
            continue;

        PlayerState->LogInfo();
    }
}

void AShooterGameModeBase::SetGameState(EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;

    OnGameStateChanged.Broadcast(NewGameState);
}

void AShooterGameModeBase::StopAllFire()
{
    for (const auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        const auto WeaponComponent = Pawn->FindComponentByClass<UShooterWeaponComponent>();
        if (!WeaponComponent)
            continue;

        WeaponComponent->StopFire();
        WeaponComponent->Zoom(false);
    }
}
