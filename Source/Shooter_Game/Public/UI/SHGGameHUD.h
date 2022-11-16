// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SHGBaseHUD.h"
#include "SHGGameHUD.generated.h"

class UShooterAnimatedUserWidget;

UCLASS()
class SHOOTER_GAME_API ASHGGameHUD : public ASHGBaseHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGAnimatedUserWidget> PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGAnimatedUserWidget> PlayerSpectatingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGAnimatedUserWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGAnimatedUserWidget> GameOverWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGAnimatedUserWidget> PopUpHintWidgetClass;

    virtual void BeginPlay() override;
    virtual void BackToRootMenu() override;

private:
    virtual void SetupWidgets() override;
};
