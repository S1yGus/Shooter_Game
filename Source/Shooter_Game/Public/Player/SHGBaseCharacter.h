// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SHGBaseCharacter.generated.h"

class USHGHealthComponent;
class USHGStaminaComponent;
class USHGBaseWeaponComponent;
class USHGBaseFXComponent;
class UPawnNoiseEmitterComponent;

UCLASS()
class SHOOTER_GAME_API ASHGBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASHGBaseCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void TurnOff() override;
    virtual void Reset() override;
    virtual void Jump() override;

    void SetColor(const FLinearColor& Color);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsSprinting() const { return bCurrentSprintState; }

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsZooming() const { return bZooming; }

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    FRotator GetViewDeltaRotator() const;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USHGHealthComponent* HealthComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USHGStaminaComponent* StaminaComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USHGBaseWeaponComponent* WeaponComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USHGBaseFXComponent* FXComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UPawnNoiseEmitterComponent* NoiseEmitterComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", Meta = (Units = "s"))
    float LifeSpanOnDeath = 20.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    FVector2D LandedDamageVelocity{800.0f, 1500.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    FVector2D LandedDamage{10.0f, 100.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName MaterialColorParameterName = "Paint Color";

    bool bZooming = false;

    inline bool WantsToSprint() const;
    inline virtual bool IsMovingForward() const;

    void StartSprint();
    void StopSprint();

    virtual void OnDeath(AController* KillerController, AController* VictimController);
    virtual void OnOutOfStamina();

private:
    bool bWantsToSprint = false;
    bool bCurrentSprintState = false;
    bool bLastSprintState = true;

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
    void OnZoom(bool bCondition);
};
