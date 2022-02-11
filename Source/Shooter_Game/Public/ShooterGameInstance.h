// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameInstance.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    FLevelData GetNewGameLevelData() const { return NewGameLevelData; }
    void SetNewGameLevelData(const FLevelData& Data) { NewGameLevelData = Data; }

    const TArray<FLevelData>& GetGameLevelsData() const { return GameLevelsData; }

    FName GetMenuLevelName() const { return MenuLevelName; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    TArray<FLevelData> GameLevelsData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName MenuLevelName = NAME_None;

private:
    FLevelData NewGameLevelData;
};
