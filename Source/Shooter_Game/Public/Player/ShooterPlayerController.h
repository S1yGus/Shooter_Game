// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SHGSettingsCoreTypes.h"
#include "ShooterCoreTypes.h"
#include "ShooterPlayerController.generated.h"

class UShooterRespawnComponent;

UCLASS()
class SHOOTER_GAME_API AShooterPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    FOnPressedEntSignature OnPressedEnt;
    FOnPressedEscSignature OnPressedEsc;

    AShooterPlayerController();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterRespawnComponent* RespawnComponent;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void OnPressedEnter();
    void OnPressedEscape();
    void OnGameStateChanged(EGameState GameState);
};
