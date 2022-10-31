// Shooter_Game, All rights reserved.

#include "SHGGameMode.h"
#include "Settings/SHGGameUserSettings.h"

void ASHGGameMode::ShowHint(EHintType HintType, float Delay)
{
    if (!HintsStatusMap.Contains(HintType))
        return;

    if (!HintsStatusMap[HintType])
        return;

    if (!HintsMap.Contains(HintType))
        return;

    if (Delay)
    {
        GetWorldTimerManager().SetTimer(    //
            DelayHintTimerHandle,           //
            [=]()
            {
                ShowPopUpHint(HintsMap[HintType]);
            },
            Delay,     //
            false);    //
    }
    else
    {
        ShowPopUpHint(HintsMap[HintType]);
    }

    HintsStatusMap[HintType] = false;

    if (const auto GameUserSettings = USHGGameUserSettings::Get())
    {
        GameUserSettings->SetHintsStatus(HintsStatusMap);
    }
}

void ASHGGameMode::StartPlay()
{
    Super::StartPlay();

    if (const auto GameUserSettings = USHGGameUserSettings::Get())
    {
        HintsStatusMap = GameUserSettings->GetHintsStatus();
        GameUserSettings->OnHintsStatusChanged.AddUObject(this, &ThisClass::OnHintsStatusChanged);
    }
}

void ASHGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorldTimerManager().ClearTimer(DelayHintTimerHandle);    // Cos the timer starts lambda.
}

void ASHGGameMode::ShowPopUpHint(const FHintData& HintData)
{
    OnShowPopUpHint.Broadcast(HintData);
}

void ASHGGameMode::OnHintsStatusChanged(const TMap<EHintType, bool>& NewHintsStatus)
{
    HintsStatusMap = NewHintsStatus;
}
