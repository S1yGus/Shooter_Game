// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterCoreTypes.h"
#include "ShooterBaseHUD.generated.h"

class UShooterBaseAnimatedUserWidget;

UCLASS()
class SHOOTER_GAME_API AShooterBaseHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void BackToRootMenu(){};

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> OptionsWidgetClass;

    UPROPERTY()
    TMap<EGameState, UShooterBaseAnimatedUserWidget*> GameWidgets;

    UPROPERTY()
    UShooterBaseAnimatedUserWidget* CurrentWidget = nullptr;

    virtual void BeginPlay() override;
    virtual void SetupWidgets();

    void OnGameStateChanged(EGameState GameState);
};
