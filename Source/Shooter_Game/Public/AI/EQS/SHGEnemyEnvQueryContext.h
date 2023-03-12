// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "SHGEnemyEnvQueryContext.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGEnemyEnvQueryContext : public UEnvQueryContext
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName EnemyBlackboardKeyName{"Enemy"};

private:
    virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
