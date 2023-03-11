// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHGBaseCoreTypes.h"
#include "SHGStaminaComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_GAME_API USHGStaminaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USHGStaminaComponent();

    FOnOutOfStaminaSignature OnOutOfStamina;
    FOnNotEnoughStaminaSignature OnNotEnoughStamina;
    FOnStaminaChangedSignature OnStaminaChanged;

    bool CanSprint();
    bool Jump();

    float GetStaminaPercent() const { return Stamina / MaxStamina; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", Meta = (ClampMin = "0.0"))
    float MaxStamina = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", Meta = (ClampMin = "0.0", Units = "s"))
    float StaminaUpdateRate = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina|Costs", Meta = (ClampMin = "0.0"))
    float StaminaJumpCost = 20.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina|Costs", Meta = (ClampMin = "0.0"))
    float StaminaSprintCost = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina|Recovery", Meta = (ClampMin = "0.0", Units = "s"))
    float StaminaRecoveryDelay = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina|Recovery", Meta = (ClampMin = "0.0"))
    float StaminaRecoveryModifier = 1.0f;

    virtual void BeginPlay() override;

private:
    float Stamina = 0.0f;
    float StaminaRecoveryCountdown = 0.0f;

    inline bool IsOutOfStamina() const { return FMath::IsNearlyZero(Stamina, StaminaSprintCost); }
    inline void SetStamina(float NewStamina);
    inline bool StaminaValidCheck(float StaminaToUse);
    bool UseStaminaWithValidCheck(float StaminaToUse);

    void OnStaminaUpdate();
};
