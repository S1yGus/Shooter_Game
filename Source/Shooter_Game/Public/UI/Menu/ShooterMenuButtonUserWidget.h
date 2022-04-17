// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterMenuButtonUserWidget.generated.h"

class UShooterButtonUserWidget;

UCLASS()
class SHOOTER_GAME_API UShooterMenuButtonUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* MainMenuButton = nullptr;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnMainMenu();
};
