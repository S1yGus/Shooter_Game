// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class SHOOTER_GAME_API AShooterAICharacter : public AShooterBaseCharacter
{
    GENERATED_BODY()

public:
    AShooterAICharacter(const FObjectInitializer& ObjectInitializer);

    UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTree;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float DistanceBetweenThreshold = 2500.0f;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* HealthWidgetComponent;

    FTimerHandle UpdateHealthWidgetVisibilityTimerHandle;

    virtual void OnHealthChanged(float Health) override;
    void UpdateHealthWidgetVisibility();

    virtual void OnDeath() override;
};
