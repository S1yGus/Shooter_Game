// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterPlayerStateInfoUserWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTER_GAME_API UShooterPlayerStateInfoUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* KillsTextBlock = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* DeathsTextBlock = nullptr;

    virtual void NativeOnInitialized() override;

private:
    void OnNewPawn(APawn* NewPawn);

    void UpdatePlayerStateInfo(int32 KillsNum, int32 DeathsNum);
};
