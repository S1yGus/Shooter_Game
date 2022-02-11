// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterMenuHUD.generated.h"

class UShooterBaseAnimatedUserWidget;

UCLASS()
class SHOOTER_GAME_API AShooterMenuHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterBaseAnimatedUserWidget> MenuWidgetClass;

    virtual void BeginPlay() override;
};
