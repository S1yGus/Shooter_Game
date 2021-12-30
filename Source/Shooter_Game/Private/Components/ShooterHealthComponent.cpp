// Shooter_Game, All rights reserved.


#include "Components/ShooterHealthComponent.h"
#include "TimerManager.h"
#include "..\..\Public\Components\ShooterHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All)

UShooterHealthComponent::UShooterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UShooterHealthComponent::TryToHeal(float HealAmount)
{
    if (IsCompletelyHealthy() || HealAmount <= 0)
        return false;
    
    SetHealth(Health + HealAmount);
    return true;
}

void UShooterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	const auto ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UShooterHealthComponent::OnTakeAnyDamage);
    }
}

void UShooterHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                              AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
        return;

    SetHealth(Health - Damage);

    if (AutoHealTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    }

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal && Health < MaxAutoHealHealth)
    {
        GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &UShooterHealthComponent::AutoHealTick,
                                               HealUpdateTime, true, HealDelay);
    }
}

void UShooterHealthComponent::AutoHealTick()
{
    SetHealth(FMath::Min(Health + HealModifier, MaxAutoHealHealth));

    if (FMath::IsNearlyEqual(Health, MaxAutoHealHealth))
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    }
}

void UShooterHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}

bool UShooterHealthComponent::IsCompletelyHealthy()
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}
