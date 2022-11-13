// Shooter_Game, All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "ShooterCoreTypes.h"
#include "SHGUICoreTypes.h"
#include "SHGHealthComponent.generated.h"

class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;
    FOnTakeDamageSignature OnTakeDamage;

    USHGHealthComponent();

    UFUNCTION(BlueprintCallable)
    inline bool IsDead() const { return FMath::IsNearlyZero(Health); }
    UFUNCTION(BlueprintCallable)
    inline float GetHealthPercent() const { return Health / MaxHealth; }
    inline float GetHealth() const { return Health; }
    bool TryToHeal(float HealAmount);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (ClampMin = "0.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    TMap<UPhysicalMaterial*, float> DamageModifiersMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|AutoHeal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|AutoHeal", Meta = (ClampMin = "0.0", EditCondition = "AutoHeal"))
    float AutoHealThreshold = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|AutoHeal", Meta = (ClampMin = "0.0", EditCondition = "AutoHeal", Units = "s"))
    float AutoHealRate = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|AutoHeal", Meta = (ClampMin = "0.0", EditCondition = "AutoHeal", Units = "s"))
    float AutoHealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|AutoHeal", Meta = (ClampMin = "0.0", EditCondition = "AutoHeal"))
    float AutoHealModifier = 1.0f;

    virtual void BeginPlay() override;

private:
    FTimerHandle AutoHealTimerHandle;

    float Health = 0.0f;
    float CurrentDamageModifier = 1.0f;

    inline bool IsCompletelyHealthy() { return FMath::IsNearlyEqual(Health, MaxHealth); }

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
    UFUNCTION()
    void OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName,
                           FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
    UFUNCTION()
    void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy,
                            AActor* DamageCauser);
    void OnAutoHeal();

    inline void SetHealth(float NewHealth);
};
