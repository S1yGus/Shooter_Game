// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShooterBaseHUD.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameHUD.generated.h"

class UShooterBaseAnimatedUserWidget;

UCLASS()
class SHOOTER_GAME_API AShooterGameHUD : public AShooterBaseHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> PlayerGameWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> PlayerSpectatingWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> GameOverWidgetClass;

    virtual void BeginPlay() override;
    virtual void BackToRootMenu() override;

private:
    virtual void SetupWidgets() override;
};
