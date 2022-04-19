// Shooter_Game, All rights reserved.

#include "Components/ShooterStaminaComponent.h"
#include "Player/ShooterBaseCharacter.h"

UShooterStaminaComponent::UShooterStaminaComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UShooterStaminaComponent::UsingStaminaValidCheck(float StaminaToUse)
{
    if (Stamina - StaminaToUse < 0.0f)
    {
        OnNotEnoughStamina.Broadcast();
        return false;
    }
    else
    {
        SetStamina(Stamina - StaminaToUse);
        CurrentStaminaRecoveryDelay = 0.0f;
        return true;
    }
}

void UShooterStaminaComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxStamina > 0.0f);
    SetStamina(MaxStamina);

    GetWorld()->GetTimerManager().SetTimer(StaminaUpdateTimerHandle, this, &UShooterStaminaComponent::OnStaminaUpdateTick, StaminaUpdateTime, true);
}

void UShooterStaminaComponent::SetStamina(float NewStamina)
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
    OnStaminaChanged.Broadcast(Stamina, GetStaminaPercent());

    if (IsOutOfStamina())
    {
        OnOutOfStamina.Broadcast();
    }
}

void UShooterStaminaComponent::UsingStamina(float StaminaToUse)
{
    SetStamina(Stamina - StaminaToUse);
}

void UShooterStaminaComponent::OnStaminaUpdateTick()
{
    if (GetOwner<AShooterBaseCharacter>()->IsSprinting())
    {
        UsingStamina(SprintStaminaFlow);
        CurrentStaminaRecoveryDelay = 0.0f;
    }
    else
    {
        if (CurrentStaminaRecoveryDelay >= StaminaRecoveryDelay)
        {
            if (FMath::IsNearlyEqual(Stamina, MaxStamina))
                return;

            UsingStamina(-StaminaRecoveryUpdateValue);
        }
        else
        {
            CurrentStaminaRecoveryDelay += StaminaUpdateTime;
        }
    }
}
