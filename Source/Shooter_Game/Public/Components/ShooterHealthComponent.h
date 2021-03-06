// Shooter_Game, All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "ShooterCoreTypes.h"
#include "ShooterHealthComponent.generated.h"

class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API UShooterHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UShooterHealthComponent();

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const { return Health / MaxHealth; }

    float GetHealth() const { return Health; }
    bool TryToHeal(float HealAmount);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (ClampMin = "0.0", ClampMax = "100.0", EditCondition = "AutoHeal"))
    float AutoHealThreshold = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (ClampMin = "0.0", ClampMax = "100.0", EditCondition = "AutoHeal"))
    float AutoHealUpdateTime = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (ClampMin = "0.0", ClampMax = "100.0", EditCondition = "AutoHeal"))
    float AutoHealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (ClampMin = "0.0", ClampMax = "100.0", EditCondition = "AutoHeal"))
    float AutoHealUpdateValue = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    TMap<UPhysicalMaterial*, float> DamageModifiersMap;

    virtual void BeginPlay() override;

private:
    float Health = 0.0f;
    float CurrentDamageModifier = 1.0f;

    FTimerHandle AutoHealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION()
    void OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent,
                           FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

    UFUNCTION()
    void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy,
                            AActor* DamageCauser);

    void AutoHealTick();
    void SetHealth(float NewHealth);
    bool IsCompletelyHealthy() { return FMath::IsNearlyEqual(Health, MaxHealth); }
    void Killed(AController* KillerController, AController* VictimController);
    void SpawnImpactIndicator(AActor* DamagedActor, float DamageAmount, const FVector& HitLocation, UPhysicalMaterial* PhysicalMaterial = nullptr);
};
