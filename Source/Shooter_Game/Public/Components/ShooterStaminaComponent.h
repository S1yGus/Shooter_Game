// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterStaminaComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API UShooterStaminaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UShooterStaminaComponent();

    FOnOutOfStaminaSignature OnOutOfStamina;
    FOnNotEnoughStaminaSignature OnNotEnoughStamina;
    FOnStaminaChangedSignature OnStaminaChanged;

    void UsingStamina(bool InUse, float StaminaInUseValue = 0.0f);
    bool UsingStaminaValidCheck(float StaminaToUse);
    float GetStaminaPercent() const { return Stamina / MaxStamina; }
    bool IsStaminaInUse() { return StaminaInUse; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", Meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxStamina = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", Meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float StaminaUpdateTime = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", Meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float StaminaRecoveryDelay = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", Meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float StaminaRecoveryUpdateValue = 1.0f;

    virtual void BeginPlay() override;

private:
    float Stamina = 0.0f;
    float StaminaUpdatValue = 0.0f;
    bool StaminaInUse = false;

    FTimerHandle StaminaUpdateTimerHandle;

    void SetStamina(float NewStamina);
    bool IsOutOfStamina() const { return FMath::IsNearlyZero(Stamina); }

    void OnStaminaUpdateTick();
};
