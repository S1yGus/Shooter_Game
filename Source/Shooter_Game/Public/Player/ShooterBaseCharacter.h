// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"

class USHGHealthComponent;
class USHGStaminaComponent;
class USHGBaseWeaponComponent;
class USHGBaseVFXComponent;

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
    USHGHealthComponent* HealthComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGStaminaComponent* StaminaComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGBaseWeaponComponent* WeaponComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGBaseVFXComponent* VFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    FVector2D LandedDamageVelocity{800.0f, 1500.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    FVector2D LandedDamageValue{10.0f, 100.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName MaterialColorParameterName = "Paint Color";

    bool WantsToSprint = false;
    bool MovingForward = false;

    void StartSprint();
    void StopSprint();

    virtual void OnHealthChanged(float Health, float HealthPercent);
    virtual void OnDeath(AController* KillerController, AController* VictimController);
    virtual void OnOutOfStamina();

private:
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
