// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SHGSettingsCoreTypes.h"
#include "SHGBaseCoreTypes.h"
#include "SHGPlayerController.generated.h"

class USHGRespawnComponent;

UCLASS()
class SHOOTER_GAME_API ASHGPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    FOnPressedEntSignature OnPressedEnt;
    FOnPressedEscSignature OnPressedEsc;

    ASHGPlayerController();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGRespawnComponent* RespawnComponent;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void OnPressedEnter();
    void OnPressedEscape();
    void OnGameStateChanged(EGameState GameState);
};
