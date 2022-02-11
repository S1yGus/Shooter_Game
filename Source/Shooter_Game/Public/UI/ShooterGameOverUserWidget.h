// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameOverUserWidget.generated.h"

class UVerticalBox;
class UShooterButtonUserWidget;
class UShooterPlayerInfoRowUserWidget;

UCLASS()
class SHOOTER_GAME_API UShooterGameOverUserWidget : public UShooterBaseAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UVerticalBox* PlayersInfoBox = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* RestartLevelButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterPlayerInfoRowUserWidget> PlayersInfoRowWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnRestartLevel();

    void OnGameStateChanged(EGameState GameState);
    void UpdatePlayersInfo();
};
