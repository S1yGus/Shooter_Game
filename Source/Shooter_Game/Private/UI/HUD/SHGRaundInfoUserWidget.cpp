// Shooter_Game, All rights reserved.

#include "UI/HUD/SHGRaundInfoUserWidget.h"
#include "Components/TextBlock.h"
#include "SHGGameModeArena.h"
#include "Kismet/KismetMathLibrary.h"

FText USHGRaundInfoUserWidget::GetCurrentRaundTime() const
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeArena>();
    if (!GameMode)
        return FText{};

    const auto Time = UKismetMathLibrary::FromSeconds(GameMode->GetCurrentRaundTime());
    const auto RaundTimeStr = FString::Printf(TEXT("%i:%i"), Time.GetMinutes(), Time.GetSeconds());
    return FText::FromString(RaundTimeStr);
}

void USHGRaundInfoUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(RaundInfoTextBlock);

    if (const auto PC = GetOwningPlayer())
    {
        PC->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawn);
    }

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeArena>())
    {
        UpdateRaundInfo(GameMode->GetCurrentRaund(), GameMode->GetRaundsNum());
    }
}

void USHGRaundInfoUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeArena>())
    {
        GameMode->OnNewRaund.AddUObject(this, &ThisClass::UpdateRaundInfo);
    }
}

void USHGRaundInfoUserWidget::UpdateRaundInfo(int32 CurrentRaund, int32 RaundsNum)
{
    const auto RaundInfoStr = FString::Printf(TEXT("%s:%i/%i"), *RaundText, CurrentRaund, RaundsNum);
    RaundInfoTextBlock->SetText(FText::FromString(RaundInfoStr));
}
