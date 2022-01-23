// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "ShooterAIPerceptionComponent.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    AActor* GetNearestActor();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName LostEnemyKeyName = "LostEnemy";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName EnemyKeyName = "Enemy";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName NewLocationKeyName = "NewLocation";

private:
    UFUNCTION()
    void CheckForEnemyLost(const FActorPerceptionUpdateInfo& UpdateInfo);
};
