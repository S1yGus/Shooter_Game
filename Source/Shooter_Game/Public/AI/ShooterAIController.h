// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class USHGAIPerceptionComponent;
class USHGRespawnComponent;

UCLASS()
class SHOOTER_GAME_API AShooterAIController : public AAIController
{
    GENERATED_BODY()

public:
    AShooterAIController();

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USHGRespawnComponent* RespawnComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName EnemyBlackboardKeyName = "Enemy";

    virtual void OnPossess(APawn* InPawn) override;

private:
    UFUNCTION()
    void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    AActor* GetFocusOnActor();
};
