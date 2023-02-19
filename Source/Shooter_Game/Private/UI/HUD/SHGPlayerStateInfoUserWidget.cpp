// Shooter_Game, All rights reserved.

#include "UI/HUD/SHGPlayerStateInfoUserWidget.h"
#include "Components/TextBlock.h"
#include "Player/SHGPlayerState.h"

void USHGPlayerStateInfoUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(KillsTextBlock);
    check(DeathsTextBlock);

    if (const auto PC = GetOwningPlayer())
    {
        PC->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawn);

        if (const auto PlayerState = PC->GetPlayerState<ASHGPlayerState>())
        {
            UpdatePlayerStateInfo(PlayerState->GetKillsNum(), PlayerState->GetDeathsNum());
        }
    }
}

void USHGPlayerStateInfoUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    if (const auto PlayerState = NewPawn->GetController()->GetPlayerState<ASHGPlayerState>())
    {
        PlayerState->OnPlayerStateChanged.AddUObject(this, &ThisClass::UpdatePlayerStateInfo);
    }
}

void USHGPlayerStateInfoUserWidget::UpdatePlayerStateInfo(int32 KillsNum, int32 DeathsNum)
{
    const auto KillsStr = FString::Printf(TEXT("%s: %d"), *KillsText, KillsNum);
    KillsTextBlock->SetText(FText::FromString(KillsStr));

    const auto DeathsStr = FString::Printf(TEXT("%s: %d"), *DeathsText, DeathsNum);
    DeathsTextBlock->SetText(FText::FromString(DeathsStr));
}
