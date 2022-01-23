// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class SHOOTER_GAME_API AShooterAICharacter : public AShooterBaseCharacter
{
    GENERATED_BODY()

public:
    AShooterAICharacter(const FObjectInitializer& ObjectInitializer);

    UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTree;

    virtual void OnDeath() override;
};
