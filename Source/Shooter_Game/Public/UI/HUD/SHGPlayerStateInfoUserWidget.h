// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGPlayerStateInfoUserWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTER_GAME_API USHGPlayerStateInfoUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* KillsTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* DeathsTextBlock;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FString KillsText{"KILLS"};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FString DeathsText{"DEATHS"};

    virtual void NativeOnInitialized() override;

private:
    void OnNewPawn(APawn* NewPawn);

    void UpdatePlayerStateInfo(int32 KillsNum, int32 DeathsNum);
};
