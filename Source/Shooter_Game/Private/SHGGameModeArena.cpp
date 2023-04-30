// Shooter_Game, All rights reserved.

#include "SHGGameModeArena.h"
#include "Player/SHGPlayerCharacter.h"
#include "Player/SHGPlayerController.h"
#include "Player/SHGPlayerState.h"
#include "Components/SHGRespawnComponent.h"
#include "Components/SHGAIWeaponComponent.h"
#include "UI/SHGGameHUD.h"
#include "AIController.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameModeArena, All, All)

ASHGGameModeArena::ASHGGameModeArena()
{
    DefaultPawnClass = ASHGPlayerCharacter::StaticClass();
    PlayerControllerClass = ASHGPlayerController::StaticClass();
    HUDClass = ASHGGameHUD::StaticClass();
    PlayerStateClass = ASHGPlayerState::StaticClass();
}

void ASHGGameModeArena::StartPlay()
{
    Super::StartPlay();

    SpawnAIControllers();
    SetTeamsID();
    StartRaund();
    SetGameState(EGameState::Game);
    ShowHint(EHintType::Startup, StartupHintDelay);
}

bool ASHGGameModeArena::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSeted = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSeted)
    {
        StopAllFire();
    }

    return PauseSeted;
}

bool ASHGGameModeArena::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetGameState(EGameState::Game);
    }

    return PauseCleared;
}

UClass* ASHGGameModeArena::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
        return AIDefaultPawnClass;

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

float ASHGGameModeArena::GetCurrentRaundTime() const
{
    return GetWorldTimerManager().IsTimerActive(RaundTimerHandle) ? GetWorldTimerManager().GetTimerRemaining(RaundTimerHandle) : 0.0f;
}

void ASHGGameModeArena::Killed(AController* KillerController, AController* VictimController)
{
    if (!KillerController || !VictimController)
        return;

    const auto KillerPlayerState = Cast<ASHGPlayerState>(KillerController->PlayerState);
    const auto VictimPlayerState = Cast<ASHGPlayerState>(VictimController->PlayerState);
    if (!KillerPlayerState || !VictimPlayerState)
        return;

    KillerPlayerState->AddKill();
    VictimPlayerState->AddDeath();

    Respawn(VictimController);
}

void ASHGGameModeArena::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

FLinearColor ASHGGameModeArena::DetermenColorByTeamID(int32 TeamID) const
{
    FLinearColor TeamColor = GameData.DefaultTeamColor;
    if (TeamID < GameData.TeamsColors.Num())
    {
        TeamColor = GameData.TeamsColors[TeamID];
    }

    return TeamColor;
}

void ASHGGameModeArena::StopAllFire()
{
    for (const auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (const auto WeaponComponent = Pawn->FindComponentByClass<USHGBaseWeaponComponent>())
        {
            WeaponComponent->StopFire();
            WeaponComponent->Zoom(false);
        }
    }
}

void ASHGGameModeArena::SpawnAIControllers()
{
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
    }
}

void ASHGGameModeArena::SetTeamsID()
{
    int32 BotID = 0;
    int32 TeamID = 0;

    for (const auto Controller : TActorRange<AController>(GetWorld()))
    {
        if (!Controller)
            continue;

        const auto SHGPlayerState = Cast<ASHGPlayerState>(Controller->PlayerState);
        if (!SHGPlayerState)
            continue;

        SHGPlayerState->SetTeamID(TeamID);
        SHGPlayerState->SetTeamColor(DetermenColorByTeamID(TeamID));
        TeamID = (TeamID + 1) % GameData.TeamsNum;
        SHGPlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot_" + FString::FromInt(BotID++));
    }
}

void ASHGGameModeArena::SetPlayerColor(AController* Controller)
{
    const auto SHGPlayerState = Cast<ASHGPlayerState>(Controller->PlayerState);
    const auto SHGBaseCharacter = Cast<ASHGBaseCharacter>(Controller->GetPawn());
    if (!SHGPlayerState || !SHGBaseCharacter)
        return;

    SHGBaseCharacter->SetColor(SHGPlayerState->GetTeamColor());
}

void ASHGGameModeArena::StartRaund()
{
    ResetPlayers();
    GetWorldTimerManager().SetTimer(RaundTimerHandle, this, &ThisClass::EndRaund, GameData.RaundTime);
    OnNewRaund.Broadcast(CurrentRaund, GameData.RaundsNum);
}

void ASHGGameModeArena::EndRaund()
{
    if (CurrentRaund != GameData.RaundsNum)
    {
        ++CurrentRaund;
        StartRaund();
    }
    else
    {
        GameOver();
    }
}

void ASHGGameModeArena::ResetPlayers()
{
    for (const auto Controller : TActorRange<AController>(GetWorld()))
    {
        ResetOnePlayer(Controller);
    }
}

void ASHGGameModeArena::ResetOnePlayer(AController* Controller)
{
    if (Controller)
    {
        if (const auto Pawn = Controller->GetPawn())
        {
            Pawn->Reset();
        }

        RestartPlayer(Controller);
        SetPlayerColor(Controller);

        if (Controller->IsPlayerController())
        {
            SetGameState(EGameState::Game);
        }
    }
}

void ASHGGameModeArena::Respawn(AController* Controller)
{
    if (!Controller || GetCurrentRaundTime() < MinRaundTimeForRespawn + GameData.RespawnTime)
        return;

    const auto RespawnComponent = Controller->FindComponentByClass<USHGRespawnComponent>();
    if (!RespawnComponent)
        return;

    RespawnComponent->StartRespawn(GameData.RespawnTime);
}

void ASHGGameModeArena::GameOver()
{
    for (const auto Controller : TActorRange<AController>(GetWorld()))
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

void ASHGGameModeArena::LogGameInfo()
{
    for (const auto Controller : TActorRange<AController>(GetWorld()))
    {
        if (!Controller)
            continue;

        const auto PlayerState = Cast<ASHGPlayerState>(Controller->PlayerState);
        if (!PlayerState)
            continue;

        PlayerState->LogInfo();
    }
}
