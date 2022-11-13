// Shooter_Game, All rights reserved.

#include "Components/SHGHealthComponent.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#define RESET_DAMAGE_MODIFIER(DamageModifier) DamageModifier = 1.0f

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All)

USHGHealthComponent::USHGHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USHGHealthComponent::TryToHeal(float HealAmount)
{
    if (IsDead() || IsCompletelyHealthy() || HealAmount <= 0.0f)
        return false;

    SetHealth(Health + HealAmount);

    if (Health >= AutoHealThreshold)
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    }

    return true;
}

void USHGHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0.0f);

    SetHealth(MaxHealth);

    if (const auto ComponentOwner = GetOwner())
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamage);
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &ThisClass::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &ThisClass::OnTakeRadialDamage);
    }
}

void USHGHealthComponent::OnTakeAnyDamage(AActor* DamagedActor,             //
                                          float Damage,                     //
                                          const UDamageType* DamageType,    //
                                          AController* InstigatedBy,        //
                                          AActor* DamageCauser)
{
    if (IsDead() || Damage <= 0.0f)
        return;

    SetHealth(Health - (Damage * CurrentDamageModifier));
    RESET_DAMAGE_MODIFIER(CurrentDamageModifier);    // Reset CurrentDamageModifier after use.

    if (IsDead())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);

        const auto ComponentOwner = GetOwner<APawn>();
        const auto OwnerController = ComponentOwner ? ComponentOwner->GetController() : nullptr;

        OnDeath.Broadcast(InstigatedBy, OwnerController);
    }
    else if (AutoHeal && (Health < AutoHealThreshold))
    {
        GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &ThisClass::OnAutoHeal, AutoHealRate, true, AutoHealDelay);
    }
}

void USHGHealthComponent::OnTakePointDamage(AActor* DamagedActor,                  //
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

    const auto CharacterMesh = DamagedCharacter->GetMesh();
    if (!CharacterMesh)
        return;

    const auto BodyInstance = CharacterMesh->GetBodyInstance(BoneName);
    if (!BodyInstance)
        return;

    const auto PhysicalMaterial = BodyInstance->GetSimplePhysicalMaterial();
    if (!PhysicalMaterial)
        return;

    CurrentDamageModifier = DamageModifiersMap.Contains(PhysicalMaterial) ? DamageModifiersMap[PhysicalMaterial] : 1.0f;

    OnTakeDamage.Broadcast(Damage * CurrentDamageModifier, HitLocation, PhysicalMaterial);
}

void USHGHealthComponent::OnTakeRadialDamage(AActor* DamagedActor,             //
                                             float Damage,                     //
                                             const UDamageType* DamageType,    //
                                             FVector Origin,                   //
                                             FHitResult HitInfo,               //
                                             AController* InstigatedBy,        //
                                             AActor* DamageCauser)
{
    OnTakeDamage.Broadcast(Damage, HitInfo.Location, nullptr);
}

void USHGHealthComponent::OnAutoHeal()
{
    SetHealth(FMath::Min(Health + AutoHealModifier, AutoHealThreshold));

    if (FMath::IsNearlyEqual(Health, AutoHealThreshold))
    {
        Health = AutoHealThreshold;
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    }
}

void USHGHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

    OnHealthChanged.Broadcast(Health, GetHealthPercent());
}
