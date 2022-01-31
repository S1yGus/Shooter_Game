// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class UShooterAIPerceptionComponent;
class UShooterRespawnComponent;

UCLASS()
class SHOOTER_GAME_API AShooterAIController : public AAIController
{
    GENERATED_BODY()

public:
    AShooterAIController();

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UShooterRespawnComponent* RespawnComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName EnemyBlackboardKeyName = "Enemy";

    virtual void OnPossess(APawn* InPawn) override;

private:
    UFUNCTION()
    void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    AActor* GetFocusOnActor();
};
