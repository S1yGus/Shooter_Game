// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterGameHUD.generated.h"

class UShooterPlayerHUDWidget;

UCLASS()
class SHOOTER_GAME_API AShooterGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterPlayerHUDWidget> PlayerHUDWidgetClass;

    virtual void BeginPlay() override;

private:
    void DrawCrossHair();
};
