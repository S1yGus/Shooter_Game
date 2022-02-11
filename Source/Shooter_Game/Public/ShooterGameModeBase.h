// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTER_GAME_API AShooterGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    FOnGameStateChangedSignature OnGameStateChanged;

    AShooterGameModeBase();

    virtual void StartPlay() override;

    UClass* GetDefaultPawnClassForController_Implementation(AController* InController);

    float GetCurrentRaund() const { return CurrentRaund; }
    float GetRaundNum() const { return GameData.RaundsNum; }
    float GetCurrentRaundTime() const;

    void Killed(AController* KillerController, AController* VictimController);
    void RespawnRequest(AController* Controller);

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TSubclassOf<APawn> AIDefaultPawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FGameData GameData;

private:
    int32 CurrentRaund = 1;
    FTimerHandle RaundTimerHandle;

    EGameState CurrentGameState = EGameState::WaitingToStart;

    void SetTeamsID();
    FLinearColor DetermenColorByTeamID(int32 TeamID) const;
    void SetPlayerColor(AController* Controller);

    void SpawnAIControllers();

    void StartRaund();
    void EndRaund();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    void Respawn(AController* Controller);

    void GameOver();
    void LogGameInfo();

    void SetGameState(EGameState NewGameState);
};