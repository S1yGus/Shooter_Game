// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameHUD.generated.h"

class UShooterBaseAnimatedUserWidget;

UCLASS()
class SHOOTER_GAME_API AShooterGameHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> GameOverWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<EGameState, UShooterBaseAnimatedUserWidget*> GameWidgets;

    UPROPERTY()
    UShooterBaseAnimatedUserWidget* CurrentWidget = nullptr;

    void DrawCrossHair();

    void InitializeWidgets();
    void OnGameStateChanged(EGameState GameState);
};
