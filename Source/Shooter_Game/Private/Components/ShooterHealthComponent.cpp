// Shooter_Game, All rights reserved.

#include "Components/ShooterHealthComponent.h"
#include "TimerManager.h"
#include "ShooterGameModeBase.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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
    if (Health >= MaxAutoHealHealth)
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);

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
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &UShooterHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UShooterHealthComponent::OnTakeRadialDamage);
    }
}

void UShooterHealthComponent::OnTakeAnyDamage(AActor* DamagedActor,             //
                                              float Damage,                     //
                                              const UDamageType* DamageType,    //
                                              AController* InstigatedBy,        //
                                              AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
        return;

    const auto DamageAmount = Damage * CurrentDamageModifier;
    SetHealth(Health - DamageAmount);
    CurrentDamageModifier = 1.0f;

    if (GetWorld()->GetTimerManager().IsTimerActive(AutoHealTimerHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    }

    if (IsDead())
    {
        const auto OwnerController = GetOwner<APawn>()->GetController();
        Killed(InstigatedBy, OwnerController);
        OnDeath.Broadcast();
    }
    else if (AutoHeal && Health < MaxAutoHealHealth)
    {
        GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &UShooterHealthComponent::AutoHealTick, HealUpdateTime, true, HealDelay);
    }
}

void UShooterHealthComponent::OnTakePointDamage(AActor* DamagedActor,                  //
                                                float Damage,                          //
                                                AController* InstigatedBy,             //
                                                FVector HitLocation,                   //
                                                UPrimitiveComponent* FHitComponent,    //
                                                FName BoneName,                        //
                                                FVector ShotFromDirection,             //
                                                const UDamageType* DamageType,         //
                                                AActor* DamageCauser)
{
    const auto DamagedCharacter = Cast<ACharacter>(DamagedActor);
    if (!DamagedCharacter)
        return;

    const auto PhysicalMaterial = DamagedCharacter->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!DamageModifiers.Contains(PhysicalMaterial))
        return;

    CurrentDamageModifier = DamageModifiers[PhysicalMaterial];
}

void UShooterHealthComponent::OnTakeRadialDamage(AActor* DamagedActor,             //
                                                 float Damage,                     //
                                                 const UDamageType* DamageType,    //
                                                 FVector Origin,                   //
                                                 FHitResult HitInfo,               //
                                                 AController* InstigatedBy,        //
                                                 AActor* DamageCauser)
{
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

void UShooterHealthComponent::Killed(AController* KillerController, AController* VictimController)
{
    const auto GameMode = GetWorld()->GetAuthGameMode<AShooterGameModeBase>();
    if (!GameMode)
        return;

    GameMode->Killed(KillerController, VictimController);
}
