// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"

class UShooterHealthComponent;
class UShooterWeaponComponent;
class UShooterBaseVFXComponent;
class AShooterBaseWeaponActor;

UCLASS()
class SHOOTER_GAME_API AShooterBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AShooterBaseCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;
    virtual void TurnOff() override;
    virtual void Reset() override;

    void SetColor(const FLinearColor& Color);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsSprinting() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = "500", ClampMax = "800"))
    float NormalWalkSpeed = 800.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = "800", ClampMax = "1400"))
    float SprintWalkSpeed = 1400.0f;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterHealthComponent* HealthComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterWeaponComponent* WeaponComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterBaseVFXComponent* VFXComponent;

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

    UFUNCTION()
    virtual void OnHealthChanged(float Health);

    virtual void OnDeath();

private:
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
