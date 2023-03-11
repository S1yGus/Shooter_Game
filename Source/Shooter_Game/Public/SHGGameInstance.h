// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SHGUICoreTypes.h"
#include "SHGGameInstance.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    void SetNewGameLevelData(const FLevelData& Data) { NewGameLevelData = Data; }
    const FLevelData& GetNewGameLevelData() const { return NewGameLevelData; }
    const TArray<FLevelData>& GetGameLevelsData() const { return GameLevelsData; }

    void OpenMainMenu();
    void StartGame();
    void RestartLevel();
    void QuitGame(APlayerController* SpecificPlayer);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TArray<FLevelData> GameLevelsData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName MenuLevelName{NAME_None};

    virtual void OnStart() override;

private:
    FLevelData NewGameLevelData;
};
