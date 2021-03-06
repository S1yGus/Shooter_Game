// Shooter_Game, All rights reserved.

#include "ShooterArenaGameMode.h"
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

AShooterArenaGameMode::AShooterArenaGameMode()
{
    DefaultPawnClass = AShooterPlayerCharacter::StaticClass();
    PlayerControllerClass = AShooterPlayerController::StaticClass();
    HUDClass = AShooterGameHUD::StaticClass();
    PlayerStateClass = AShooterPlayerState::StaticClass();
}

void AShooterArenaGameMode::StartPlay()
{
    Super::StartPlay();

    SpawnAIControllers();
    SetTeamsID();
    StartRaund();

    SetGameState(EGameState::InGame);
}

UClass* AShooterArenaGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
        return AIDefaultPawnClass;

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

float AShooterArenaGameMode::GetCurrentRaundTime() const
{
    if (!GetWorldTimerManager().IsTimerActive(RaundTimerHandle))
        return 0.0f;

    return GetWorldTimerManager().GetTimerRemaining(RaundTimerHandle);
}

void AShooterArenaGameMode::Killed(AController* KillerController, AController* VictimController)
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

void AShooterArenaGameMode::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

bool AShooterArenaGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const bool PauseSeted = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSeted)
    {
        StopAllFire();
        SetGameState(EGameState::Pause);
    }

    return PauseSeted;
}

bool AShooterArenaGameMode::ClearPause()
{
    const bool PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetGameState(EGameState::InGame);
    }

    return PauseCleared;
}

void AShooterArenaGameMode::InPause()
{
    SetGameState(EGameState::Pause);
}

void AShooterArenaGameMode::InSpectating()
{
    SetGameState(EGameState::InSpectating);
}

void AShooterArenaGameMode::SetTeamsID()
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

FLinearColor AShooterArenaGameMode::DetermenColorByTeamID(int32 TeamID) const
{
    FLinearColor TeamColor = GameData.DefaultTeamColor;
    if (TeamID <= GameData.TeamsColors.Num() - 1)
    {
        TeamColor = GameData.TeamsColors[TeamID];
    }

    return TeamColor;
}

void AShooterArenaGameMode::SetPlayerColor(AController* Controller)
{
    const auto ShooterPlayerState = Cast<AShooterPlayerState>(Controller->PlayerState);
    const auto ShooterBaseCharacter = Cast<AShooterBaseCharacter>(Controller->GetPawn());
    if (!ShooterPlayerState || !ShooterBaseCharacter)
        return;

    ShooterBaseCharacter->SetColor(DetermenColorByTeamID(ShooterPlayerState->GetTeamID()));
}

void AShooterArenaGameMode::SpawnAIControllers()
{
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
    }
}

void AShooterArenaGameMode::StartRaund()
{
    ResetPlayers();

    GetWorldTimerManager().SetTimer(RaundTimerHandle, this, &AShooterArenaGameMode::EndRaund, GameData.RaundTime);

    OnNewRaund.Broadcast(CurrentRaund, GameData.RaundsNum);
}

void AShooterArenaGameMode::EndRaund()
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

void AShooterArenaGameMode::ResetPlayers()
{
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void AShooterArenaGameMode::ResetOnePlayer(AController* Controller)
{
    if (!Controller)
        return;

    const auto Pawn = Controller->GetPawn();
    if (Pawn)
    {
        Pawn->Reset();
    }

    RestartPlayer(Controller);
    SetPlayerColor(Controller);

    if (Controller->IsPlayerController())
    {
        SetGameState(EGameState::InGame);
    }
}

void AShooterArenaGameMode::Respawn(AController* Controller)
{
    if (!Controller || GetCurrentRaundTime() < MinRaundTimeForRespawn + GameData.RespawnTime)
        return;

    const auto RespawnComponent = Controller->FindComponentByClass<UShooterRespawnComponent>();
    if (!RespawnComponent)
        return;

    RespawnComponent->StartRespawn(GameData.RespawnTime);
}

void AShooterArenaGameMode::GameOver()
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

void AShooterArenaGameMode::LogGameInfo()
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

void AShooterArenaGameMode::StopAllFire()
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