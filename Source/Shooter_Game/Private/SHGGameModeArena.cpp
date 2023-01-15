// Shooter_Game, All rights reserved.

#include "SHGGameModeArena.h"
#include "Player/ShooterPlayerCharacter.h"
#include "Player/SHGPlayerController.h"
#include "Player/ShooterPlayerState.h"
#include "Components/SHGRespawnComponent.h"
#include "Components/SHGAIWeaponComponent.h"
#include "UI/SHGGameHUD.h"
#include "AIController.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(GameModeLog, All, All)

constexpr static int32 MinRaundTimeForRespawn = 1;

ASHGGameModeArena::ASHGGameModeArena()
{
    DefaultPawnClass = AShooterPlayerCharacter::StaticClass();
    PlayerControllerClass = ASHGPlayerController::StaticClass();
    HUDClass = ASHGGameHUD::StaticClass();
    PlayerStateClass = AShooterPlayerState::StaticClass();
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

UClass* ASHGGameModeArena::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
        return AIDefaultPawnClass;

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

float ASHGGameModeArena::GetCurrentRaundTime() const
{
    if (!GetWorldTimerManager().IsTimerActive(RaundTimerHandle))
        return 0.0f;

    return GetWorldTimerManager().GetTimerRemaining(RaundTimerHandle);
}

void ASHGGameModeArena::Killed(AController* KillerController, AController* VictimController)
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

void ASHGGameModeArena::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
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

void ASHGGameModeArena::SetTeamsID()
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

FLinearColor ASHGGameModeArena::DetermenColorByTeamID(int32 TeamID) const
{
    FLinearColor TeamColor = GameData.DefaultTeamColor;
    if (TeamID <= GameData.TeamsColors.Num() - 1)
    {
        TeamColor = GameData.TeamsColors[TeamID];
    }

    return TeamColor;
}

void ASHGGameModeArena::SetPlayerColor(AController* Controller)
{
    const auto ShooterPlayerState = Cast<AShooterPlayerState>(Controller->PlayerState);
    const auto ShooterBaseCharacter = Cast<AShooterBaseCharacter>(Controller->GetPawn());
    if (!ShooterPlayerState || !ShooterBaseCharacter)
        return;

    ShooterBaseCharacter->SetColor(DetermenColorByTeamID(ShooterPlayerState->GetTeamID()));
}

void ASHGGameModeArena::SpawnAIControllers()
{
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
    }
}

void ASHGGameModeArena::StartRaund()
{
    ResetPlayers();

    GetWorldTimerManager().SetTimer(RaundTimerHandle, this, &ThisClass::EndRaund, GameData.RaundTime);

    OnNewRaund.Broadcast(CurrentRaund, GameData.RaundsNum);
}

void ASHGGameModeArena::EndRaund()
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

void ASHGGameModeArena::ResetPlayers()
{
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASHGGameModeArena::ResetOnePlayer(AController* Controller)
{
    if (!Controller)
        return;

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

void ASHGGameModeArena::StopAllFire()
{
    for (const auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        const auto WeaponComponent = Pawn->FindComponentByClass<USHGBaseWeaponComponent>();
        if (!WeaponComponent)
            continue;

        WeaponComponent->StopFire();
        WeaponComponent->Zoom(false);
    }
}
