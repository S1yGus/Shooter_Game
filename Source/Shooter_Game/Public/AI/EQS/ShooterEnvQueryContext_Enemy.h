// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "ShooterEnvQueryContext_Enemy.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterEnvQueryContext_Enemy : public UEnvQueryContext
{
    GENERATED_BODY()

    virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName EnemyBlackboardKeyName = "Enemy";
};
