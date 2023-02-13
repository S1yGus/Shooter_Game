// Shooter_Game, All rights reserved.

#include "Components/SHGStaminaComponent.h"
#include "Player/SHGBaseCharacter.h"

USHGStaminaComponent::USHGStaminaComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USHGStaminaComponent::CanSprint()
{
    return StaminaValidCheck(StaminaSprintCost);
}

bool USHGStaminaComponent::Jump()
{
    return UseStaminaWithValidCheck(StaminaJumpCost);
}

void USHGStaminaComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxStamina > 0.0f);

    SetStamina(MaxStamina);
    StaminaRecoveryCountdown = StaminaRecoveryDelay;

    FTimerHandle StaminaUpdateTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(StaminaUpdateTimerHandle, this, &ThisClass::OnStaminaUpdate, StaminaUpdateRate, true);
}

void USHGStaminaComponent::SetStamina(float NewStamina)
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

    OnStaminaChanged.Broadcast(Stamina, GetStaminaPercent());

    if (IsOutOfStamina())
    {
        OnOutOfStamina.Broadcast();
    }
}

bool USHGStaminaComponent::StaminaValidCheck(float StaminaToUse)
{
    if (Stamina - StaminaToUse < 0.0f)
    {
        OnNotEnoughStamina.Broadcast();

        return false;
    }
    else
    {
        return true;
    }
}

bool USHGStaminaComponent::UseStaminaWithValidCheck(float StaminaToUse)
{
    if (StaminaValidCheck(StaminaToUse))
    {
        SetStamina(Stamina - StaminaToUse);
        StaminaRecoveryCountdown = StaminaRecoveryDelay;

        return true;
    }
    else
    {
        return false;
    }
}

void USHGStaminaComponent::OnStaminaUpdate()
{
    if (const auto Owner = GetOwner<ASHGBaseCharacter>(); Owner && Owner->IsSprinting())
    {
        UseStaminaWithValidCheck(StaminaSprintCost);
    }
    else
    {
        if (StaminaRecoveryCountdown <= 0.0f)
        {
            if (FMath::IsNearlyEqual(Stamina, MaxStamina))
            {
                Stamina = MaxStamina;
            }
            else
            {
                SetStamina(Stamina + StaminaRecoveryModifier);
            }
        }
        else
        {
            StaminaRecoveryCountdown -= StaminaUpdateRate;
        }
    }
}
