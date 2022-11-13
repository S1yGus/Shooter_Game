// Shooter_Game, All rights reserved.

#include "Components/SHGStaminaComponent.h"

USHGStaminaComponent::USHGStaminaComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USHGStaminaComponent::Sprint(bool bIsSprinting)
{
    if (bIsSprinting)
    {
        if (UsingStaminaValidCheck(StaminaSprintCost))
        {
            bSprinting = true;

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        bSprinting = false;

        return true;
    }
}

bool USHGStaminaComponent::Jump()
{
    return UsingStaminaValidCheck(StaminaJumpCost);
}

void USHGStaminaComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxStamina > 0.0f);

    SetStamina(MaxStamina);
    StaminaRecoveryCountdown = StaminaRecoveryDelay;
}

void USHGStaminaComponent::SetStamina(float NewStamina)
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

    OnStaminaChanged.Broadcast(Stamina, GetStaminaPercent());

    if (IsOutOfStamina())
    {
        bSprinting = false;
        OnOutOfStamina.Broadcast();
    }
}

bool USHGStaminaComponent::UsingStaminaValidCheck(float StaminaToUse)
{
    if (Stamina - StaminaToUse < 0.0f)
    {
        OnNotEnoughStamina.Broadcast();

        return false;
    }
    else
    {
        SetStamina(Stamina - StaminaToUse);
        StaminaRecoveryCountdown = StaminaRecoveryDelay;
        if (!GetWorld()->GetTimerManager().IsTimerActive(StaminaUpdateTimerHandle))
        {
            GetWorld()->GetTimerManager().SetTimer(StaminaUpdateTimerHandle, this, &ThisClass::OnStaminaUpdate, StaminaUpdateRate, true);
        }

        return true;
    }
}

void USHGStaminaComponent::OnStaminaUpdate()
{
    if (bSprinting)
    {
        SetStamina(Stamina - StaminaSprintCost);
        StaminaRecoveryCountdown = StaminaRecoveryDelay;
    }
    else
    {
        if (StaminaRecoveryCountdown <= 0.0f)
        {
            if (FMath::IsNearlyEqual(Stamina, MaxStamina))
            {
                Stamina = MaxStamina;
                GetWorld()->GetTimerManager().ClearTimer(StaminaUpdateTimerHandle);

                return;
            }

            SetStamina(Stamina + StaminaRecoveryModifier);
        }
        else
        {
            StaminaRecoveryCountdown -= StaminaUpdateRate;
        }
    }
}
