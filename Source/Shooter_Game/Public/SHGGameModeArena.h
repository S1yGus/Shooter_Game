// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SHGGameMode.h"
#include "SHGBaseCoreTypes.h"
#include "SHGGameModeArena.generated.h"

class AAIController;

UCLASS()
class SHOOTER_GAME_API ASHGGameModeArena : public ASHGGameMode
{
    GENERATED_BODY()

public:
    FOnNewRaundSignature OnNewRaund;

    ASHGGameModeArena();

    virtual void StartPlay() override;

    UClass* GetDefaultPawnClassForController_Implementation(AController* InController);

    float GetCurrentRaund() const { return CurrentRaund; }
    float GetRaundsNum() const { return GameData.RaundsNum; }
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
    FArenaData GameData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    float StartupHintDelay = 2.0f;

private:
    FTimerHandle RaundTimerHandle;

    int32 CurrentRaund = 1;

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

    void StopAllFire();
};
