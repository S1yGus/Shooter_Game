// Shooter_Game, All rights reserved.

#include "UI/HUD/SHGStatsUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/SHGHealthComponent.h"
#include "Components/ShooterStaminaComponent.h"

void USHGStatsUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(HealthProgressBar);
    check(StaminaProgressBar);

    if (const auto PC = GetOwningPlayer())
    {
        PC->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawn);
    }
}

void USHGStatsUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    if (const auto HealthComponent = NewPawn->FindComponentByClass<USHGHealthComponent>())
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChanged);
        UpdateHealthProgressBar(HealthComponent->GetHealthPercent());
    }

    if (const auto StaminaComponent = NewPawn->FindComponentByClass<UShooterStaminaComponent>())
    {
        StaminaComponent->OnStaminaChanged.AddUObject(this, &ThisClass::OnStaminaChanged);
        StaminaComponent->OnOutOfStamina.AddUObject(this, &ThisClass::OnNotEnoughStamina);
        StaminaComponent->OnNotEnoughStamina.AddUObject(this, &ThisClass::OnNotEnoughStamina);
        UpdateStaminaProgressBar(StaminaComponent->GetStaminaPercent());
    }
}

void USHGStatsUserWidget::OnHealthChanged(float Health, float HealthPercent)
{
    UpdateHealthProgressBar(HealthPercent);
}

void USHGStatsUserWidget::OnStaminaChanged(float Stamina, float StaminaPercent)
{
    UpdateStaminaProgressBar(StaminaPercent);
}

void USHGStatsUserWidget::OnNotEnoughStamina()
{
    ShowAnimation(NotEnoughStaminaAnimation);
}

void USHGStatsUserWidget::ShowAnimation(UWidgetAnimation* Animation)
{
    if (IsAnimationPlaying(Animation))
        return;

    PlayAnimation(Animation);
}

void USHGStatsUserWidget::UpdateHealthProgressBar(const float HealthPercent)
{
    HealthProgressBar->SetPercent(HealthPercent);
    HealthProgressBar->SetFillColorAndOpacity(HealthPercent > HealthColorThreshold ? NormalHealthColor : LowHealthColor);
}

void USHGStatsUserWidget::UpdateStaminaProgressBar(const float StaminaPercent)
{
    StaminaProgressBar->SetPercent(StaminaPercent);
    StaminaProgressBar->SetFillColorAndOpacity(StaminaPercent > StaminaColorThreshold ? NormalStaminaColor : LowStaminaColor);
}