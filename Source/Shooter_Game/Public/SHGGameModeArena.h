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
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    UClass* GetDefaultPawnClassForController_Implementation(AController* InController);
    float GetCurrentRaund() const { return CurrentRaund; }
    float GetRaundsNum() const { return GameData.RaundsNum; }
    inline float GetCurrentRaundTime() const;

    void Killed(AController* KillerController, AController* VictimController);
    void RespawnRequest(AController* Controller);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TSubclassOf<APawn> AIDefaultPawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FArenaData GameData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (Units = "s"))
    float MinRaundTimeForRespawn = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints", Meta = (Units = "s"))
    float StartupHintDelay = 2.0f;

private:
    FTimerHandle RaundTimerHandle;
    int32 CurrentRaund = 1;

    FLinearColor DetermenColorByTeamID(int32 TeamID) const;
    void StopAllFire();

    void SpawnAIControllers();
    void SetTeamsID();
    void SetPlayerColor(AController* Controller);

    void StartRaund();
    void EndRaund();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);
    void Respawn(AController* Controller);

    void GameOver();
    void LogGameInfo();
};
