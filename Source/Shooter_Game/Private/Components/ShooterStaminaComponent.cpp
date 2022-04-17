// Shooter_Game, All rights reserved.

#include "Components/ShooterStaminaComponent.h"

UShooterStaminaComponent::UShooterStaminaComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UShooterStaminaComponent::UsingStamina(bool InUse, float StaminaInUseValue)
{
    if (InUse && StaminaInUseValue >= 0)
        return;

    StaminaInUse = InUse;
    StaminaUpdatValue = InUse ? StaminaInUseValue : StaminaRecoveryUpdateValue;
    const auto Delay = InUse ? 0.0f : StaminaRecoveryDelay;

    GetWorld()->GetTimerManager().SetTimer(StaminaUpdateTimerHandle, this, &UShooterStaminaComponent::OnStaminaUpdateTick, StaminaUpdateTime, true, Delay);
}

bool UShooterStaminaComponent::UsingStaminaValidCheck(float StaminaToUse)
{
    if (Stamina - StaminaToUse < 0)
    {
        OnNotEnoughStamina.Broadcast();
        return false;
    }
    else
    {
        SetStamina(Stamina - StaminaToUse);
        if (!StaminaInUse)
        {
            UsingStamina(false);
        }

        return true;
    }
}

void UShooterStaminaComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxStamina > 0.0f);
    SetStamina(MaxStamina);
}

void UShooterStaminaComponent::SetStamina(float NewStamina)
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
    OnStaminaChanged.Broadcast(Stamina, GetStaminaPercent());

    if (IsOutOfStamina())
    {
        OnOutOfStamina.Broadcast();
        UsingStamina(false);
    }
}

void UShooterStaminaComponent::OnStaminaUpdateTick()
{
    SetStamina(Stamina + StaminaUpdatValue);

    if (FMath::IsNearlyEqual(Stamina, MaxStamina))
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaUpdateTimerHandle);
    }
}
