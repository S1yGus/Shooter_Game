// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"

class UShooterHealthComponent;
class UShooterStaminaComponent;
class UShooterWeaponComponent;
class UShooterBaseVFXComponent;

UCLASS()
class SHOOTER_GAME_API AShooterBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AShooterBaseCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;
    virtual void TurnOff() override;
    virtual void Reset() override;
    virtual void Jump() override;

    void SetColor(const FLinearColor& Color);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsSprinting() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    FRotator GetViewDeltaRotator() const;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterHealthComponent* HealthComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterStaminaComponent* StaminaComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterWeaponComponent* WeaponComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterBaseVFXComponent* VFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = "0.1", ClampMax = "100.0"))
    float JumpStaminaCost = 20.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = "-0.1", ClampMax = "-100.0"))
    float SprintStaminaFlow = -2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(800.0f, 1500.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    FVector2D LandedDamageValue = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName MaterialColorParameterName = "Paint Color";

    bool WantsToSprint = false;
    bool MovingForward = false;

    void StartSprint();
    void StopSprint();

    virtual void OnHealthChanged(float Health, float HealthPercent);
    virtual void OnDeath();
    virtual void OnOutOfStamina();

private:
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
