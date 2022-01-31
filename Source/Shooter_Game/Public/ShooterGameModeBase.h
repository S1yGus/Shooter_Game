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
    AShooterGameModeBase();

    virtual void StartPlay() override;

    UClass* GetDefaultPawnClassForController_Implementation(AController* InController);

    float GetCurrentRaund() const { return CurrentRaund; }
    float GetRaundNum() const { return GameData.RaundsNum; }
    float GetCurrentRaundTime() const;

    void Killed(AController* KillerController, AController* VictimController);
    void RespawnRequest(AController* Controller);

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

    void SetTeamsID();
    FLinearColor DetermenColorByID(int32 TeamID);
    void SetPlayerColor(AController* Controller);

    void SpawnAIControllers();

    void StartRaund();
    void EndRaund();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    void Respawn(AController* Controller);

    void GameOver();
    void LogGameInfo();
};