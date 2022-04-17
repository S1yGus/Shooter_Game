// Shooter_Game, All rights reserved.

#include "UI/HUD/ShooterStatsUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterStaminaComponent.h"

void UShooterStatsUserWidget::NativeOnInitialized()
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
        return;

    Controller->GetOnNewPawnNotifier().AddUObject(this, &UShooterStatsUserWidget::OnNewPawn);
}

void UShooterStatsUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    if (const auto HealthComponent = NewPawn->FindComponentByClass<UShooterHealthComponent>())
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &UShooterStatsUserWidget::OnHealthChanged);
    }

    if (const auto StaminaComponent = NewPawn->FindComponentByClass<UShooterStaminaComponent>())
    {
        StaminaComponent->OnStaminaChanged.AddUObject(this, &UShooterStatsUserWidget::OnStaminaChanged);
        StaminaComponent->OnOutOfStamina.AddUObject(this, &UShooterStatsUserWidget::OnOutOfStamina);
        StaminaComponent->OnNotEnoughStamina.AddUObject(this, &UShooterStatsUserWidget::OnNotEnoughStamina);
    }

    UpdateHealthProgressBar(GetHelthPercent());
    UpdateStaminaProgressBar(GetStaminaPercent());
}

void UShooterStatsUserWidget::OnHealthChanged(float Health, float HealthPercent)
{
    UpdateHealthProgressBar(HealthPercent);
}

void UShooterStatsUserWidget::OnStaminaChanged(float Stamina, float StaminaPercent)
{
    UpdateStaminaProgressBar(StaminaPercent);
}

void UShooterStatsUserWidget::OnNotEnoughStamina()
{
    ShowAnimation(NotEnoughStaminaAnimation);
}

void UShooterStatsUserWidget::OnOutOfStamina()
{
    ShowAnimation(NotEnoughStaminaAnimation);
}

void UShooterStatsUserWidget::ShowAnimation(UWidgetAnimation* Animation)
{
    if (IsAnimationPlaying(Animation))
        return;

    PlayAnimation(Animation);
}

float UShooterStatsUserWidget::GetHelthPercent() const
{
    if (!GetOwningPlayerPawn())
        return 0.0f;

    const auto HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterHealthComponent>();
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

float UShooterStatsUserWidget::GetStaminaPercent() const
{
    if (!GetOwningPlayerPawn())
        return 0.0f;

    const auto StaminaComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterStaminaComponent>();
    if (!StaminaComponent)
        return 0.0f;

    return StaminaComponent->GetStaminaPercent();
}

void UShooterStatsUserWidget::UpdateHealthProgressBar(const float HealthPercent)
{
    if (!HealthProgressBar)
        return;

    HealthProgressBar->SetPercent(HealthPercent);
    HealthProgressBar->SetFillColorAndOpacity(HealthPercent > HealthColorThreshold ? NormalHealthColor : LowHealthColor);
}

void UShooterStatsUserWidget::UpdateStaminaProgressBar(const float StaminaPercent)
{
    if (!StaminaProgressBar)
        return;

    StaminaProgressBar->SetPercent(StaminaPercent);
    StaminaProgressBar->SetFillColorAndOpacity(StaminaPercent > StaminaColorThreshold ? NormalStaminaColor : LowStaminaColor);
}