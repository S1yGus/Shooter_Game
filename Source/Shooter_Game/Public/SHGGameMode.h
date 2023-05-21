// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SHGGameModeBase.h"
#include "SHGUICoreTypes.h"
#include "SHGGameMode.generated.h"

UCLASS(Abstract)
class SHOOTER_GAME_API ASHGGameMode : public ASHGGameModeBase
{
    GENERATED_BODY()

public:
    FOnShowPopUpHintSignature OnShowPopUpHint;

    void ShowHint(EHintType HintType, float Delay = 0.0f);

    virtual void StartPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hints")
    TMap<EHintType, FHintData> HintsMap;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    FTimerHandle DelayHintTimerHandle;
    TMap<EHintType, bool> HintsStatusMapCached;

    void ShowPopUpHint(const FHintData& HintData);

    void OnHintsStatusChanged(const TMap<EHintType, bool>& NewHintsStatus);
};