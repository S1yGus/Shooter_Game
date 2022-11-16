// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SHGBaseCoreTypes.h"
#include "SHGBaseHUD.generated.h"

class USHGAnimatedUserWidget;

UCLASS(Abstract)
class SHOOTER_GAME_API ASHGBaseHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void BackToRootMenu(){};

protected:
    UPROPERTY()
    TMap<EGameState, USHGAnimatedUserWidget*> GameWidgets;
    UPROPERTY()
    USHGAnimatedUserWidget* CurrentWidget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGAnimatedUserWidget> OptionsWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGAnimatedUserWidget> OptionsWarningWidgetClass;

    virtual void BeginPlay() override;
    virtual void SetupWidgets();

private:
    void OnGameStateChanged(EGameState GameState);
};
