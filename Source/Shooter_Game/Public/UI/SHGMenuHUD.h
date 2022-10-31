// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SHGBaseHUD.h"
#include "SHGMenuHUD.generated.h"

class USHGAnimatedUserWidget;

UCLASS()
class SHOOTER_GAME_API ASHGMenuHUD : public ASHGBaseHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGAnimatedUserWidget> MainMenuWidgetClass;

    virtual void BeginPlay() override;
    virtual void BackToRootMenu() override;

private:
    virtual void SetupWidgets() override;
};
