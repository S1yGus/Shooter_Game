// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterBaseHUD.h"
#include "ShooterMenuHUD.generated.h"

class UShooterBaseAnimatedUserWidget;

UCLASS()
class SHOOTER_GAME_API AShooterMenuHUD : public AShooterBaseHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> MainMenuWidgetClass;

    virtual void BeginPlay() override;
    virtual void BackToRootMenu() override;

private:
    virtual void SetupWidgets() override;
};
