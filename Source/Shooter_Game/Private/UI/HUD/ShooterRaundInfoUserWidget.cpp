// Shooter_Game, All rights reserved.

#include "UI/HUD/ShooterRaundInfoUserWidget.h"
#include "Components/TextBlock.h"
#include "ShooterArenaGameMode.h"
#include "Kismet/KismetMathLibrary.h"

FText UShooterRaundInfoUserWidget::GetCurrentRaundTime() const
{
    const auto GameMode = GetWorld()->GetAuthGameMode<AShooterArenaGameMode>();
    if (!GameMode)
        return FText();

    const auto Time = UKismetMathLibrary::FromSeconds(GameMode->GetCurrentRaundTime());
    const auto RaundTimeStr = FString::Printf(TEXT("%i:%i"), Time.GetMinutes(), Time.GetSeconds());
    return FText::FromString(RaundTimeStr);
}

void UShooterRaundInfoUserWidget::NativeOnInitialized()
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
        return;

    Controller->GetOnNewPawnNotifier().AddUObject(this, &UShooterRaundInfoUserWidget::OnNewPawn);
}

void UShooterRaundInfoUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    if (const auto GameMode = GetWorld()->GetAuthGameMode<AShooterArenaGameMode>())
    {
        GameMode->OnNewRaund.AddUObject(this, &UShooterRaundInfoUserWidget::UpdateRaundInfo);
    }
}

void UShooterRaundInfoUserWidget::UpdateRaundInfo(int32 CurrentRaund, int32 RaundsNum)
{
    if (!RaundInfoTextBlock)
        return;

    const auto RaundInfoStr = FString::Printf(TEXT("Raund: %i/%i"), CurrentRaund, RaundsNum);
    RaundInfoTextBlock->SetText(FText::FromString(RaundInfoStr));
}
