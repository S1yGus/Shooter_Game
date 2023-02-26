// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/SHGBaseCharacter.h"
#include "SHGAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class SHOOTER_GAME_API ASHGAICharacter : public ASHGBaseCharacter
{
    GENERATED_BODY()

public:
    ASHGAICharacter(const FObjectInitializer& ObjectInitializer);

    UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UWidgetComponent* HealthWidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTree;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float HealthWidgetVisibilityDistance = 2500.0f;

private:
    virtual void OnDeath(AController* KillerController, AController* VictimController) override;

    void OnHealthChanged(float Health, float HealthPercent);
    void OnUpdateHealthWidget();
};
