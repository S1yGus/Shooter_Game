// Shooter_Game, All rights reserved.

#include "UI/HUD/ShooterPlayerStateInfoUserWidget.h"
#include "Components/TextBlock.h"
#include "Player/ShooterPlayerState.h"

void UShooterPlayerStateInfoUserWidget::NativeOnInitialized()
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
        return;

    Controller->GetOnNewPawnNotifier().AddUObject(this, &UShooterPlayerStateInfoUserWidget::OnNewPawn);
}

void UShooterPlayerStateInfoUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    if (NewPawn->GetController() && NewPawn->GetController()->GetPlayerState<AShooterPlayerState>())
    {
        const auto PlayerState = NewPawn->GetController()->GetPlayerState<AShooterPlayerState>();
        PlayerState->OnPlayerStateChanged.AddUObject(this, &UShooterPlayerStateInfoUserWidget::UpdatePlayerStateInfo);
    }
}

void UShooterPlayerStateInfoUserWidget::UpdatePlayerStateInfo(int32 KillsNum, int32 DeathsNum)
{
    if (KillsTextBlock)
    {
        const auto KillsStr = FString::Printf(TEXT("Kills: %i"), KillsNum);
        KillsTextBlock->SetText(FText::FromString(KillsStr));
    }

    if (DeathsTextBlock)
    {
        const auto DeathsStr = FString::Printf(TEXT("Deaths: %i"), DeathsNum);
        DeathsTextBlock->SetText(FText::FromString(DeathsStr));
    }
}
